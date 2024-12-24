#include "pch.h"
#include "CsScriptClass.h"

#include "Minty/Script/CS/CsScriptAssembly.h"
#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/Script/CS/CsScriptMethod.h"

using namespace Minty;

Minty::CsScriptClass::CsScriptClass(ScriptClassBuilder const& builder)
	: ScriptClass(builder)
	, mp_class(nullptr)
{
	// get assembly
	Ref<CsScriptAssembly> assembly = static_cast<Ref<CsScriptAssembly>>(builder.assembly);

	// get image
	MonoImage* image = mono_assembly_get_image(assembly->get_assembly());

	// get class
	mp_class = mono_class_from_name(image, builder.namespaceName.c_str(), builder.className.c_str());
}

Bool Minty::CsScriptClass::has_method(String const& name, Int const parameterCount) const
{
	return static_cast<Bool>(mono_class_get_method_from_name(mp_class, name.c_str(), parameterCount));
}

Bool Minty::CsScriptClass::is_derived_from(Ref<ScriptClass> const klass) const
{
	MonoClass* current = mp_class;

	MonoClass* target = static_cast<MonoClass*>(klass->get_native());

	while (current != nullptr && !CsScriptEngine::check_equal(current, target))
	{
		current = mono_class_get_parent(current);
	}

	// if no more parents, returns null, otherise returns True
	return static_cast<Bool>(current);
}

Owner<ScriptMethod> Minty::CsScriptClass::create_method(String const& name, Int const parameterCount)
{
	MonoClass* klass = mp_class;

	ScriptMethodBuilder builder{};
	builder.object = nullptr; // no object for static method

	MonoMethod* method;
	while (klass != nullptr)
	{
		if (method = mono_class_get_method_from_name(klass, name.c_str(), parameterCount))
		{
			return Owner<CsScriptMethod>(builder, method);
		}

		// check inherited values as well
		klass = mono_class_get_parent(klass);
	}
	
	return {};
}
