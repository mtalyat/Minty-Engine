#include "pch.h"
#include "Minty/Scripting/M_ScriptClass.h"

#include "Minty/Libraries/M_Mono.h"

#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Scripting/M_ScriptArguments.h"

using namespace Minty;

Minty::ScriptClass::ScriptClass(String const& namespaceName, String const& className, ScriptAssembly& assembly)
	: _namespace(namespaceName)
	, _name(className)
	, _assembly(&assembly)
	, _class()
{
	MonoImage* image = mono_assembly_get_image(assembly._assembly);
	_class = mono_class_from_name(image, namespaceName.c_str(), className.c_str());
}

String Minty::ScriptClass::get_full_name() const
{
	return ScriptEngine::get_full_name(_namespace, _name);
}

bool Minty::ScriptClass::is_derived_from(ScriptClass const& other) const
{
	MonoClass* current = _class;

	while (current != nullptr && current != other._class)
	{
		current = mono_class_get_parent(current);
	}

	return current;
}

void Minty::ScriptClass::invoke(String const& name) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name);

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(nullptr, method);
}

void Minty::ScriptClass::invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name, static_cast<int>(arguments.size()));

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(nullptr, method, arguments.get_values());
}

bool Minty::ScriptClass::try_invoke(String const& name) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name);

	if (!method) return false;

	// invoke it
	engine.invoke_method(nullptr, method);

	return true;
}

bool Minty::ScriptClass::try_invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name, static_cast<int>(arguments.size()));

	if (!method) return false;

	// invoke it
	engine.invoke_method(nullptr, method, arguments.get_values());

	return true;
}

ScriptObject Minty::ScriptClass::create_object(UUID const id) const
{
	return ScriptEngine::instance().create_object(*this, id);
}

bool Minty::ScriptClass::has_method(String const& name) const
{
	return ScriptEngine::instance().get_method(_class, name);
}

MonoType* Minty::ScriptClass::get_type() const
{
	String fullName = get_full_name();
	return mono_reflection_type_from_name(fullName.data(), _assembly->_image);
}
