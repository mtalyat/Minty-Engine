#include "pch.h"
#include "scripting/M_ScriptClass.h"

#include "libraries/M_Mono.h"

#include "scripting/M_ScriptObject.h"
#include "scripting/M_ScriptEngine.h"
#include "scripting/M_ScriptArguments.h"

using namespace minty;

minty::ScriptClass::ScriptClass(String const& namespaceName, String const& className, ScriptAssembly& assembly)
	: _namespace(namespaceName)
	, _name(className)
	, _assembly(&assembly)
	, _class()
{
	MonoImage* image = mono_assembly_get_image(assembly._assembly);
	_class = mono_class_from_name(image, namespaceName.c_str(), className.c_str());
}

ScriptAssembly& minty::ScriptClass::get_assembly() const
{
	return *_assembly;
}

String const& minty::ScriptClass::get_namespace() const
{
	return _namespace;
}

String const& minty::ScriptClass::get_name() const
{
	return _name;
}

String minty::ScriptClass::get_full_name() const
{
	return ScriptEngine::get_full_name(_namespace, _name);
}

bool minty::ScriptClass::is_derived_from(ScriptClass const& other) const
{
	MonoClass* current = _class;

	while (current != nullptr && current != other._class)
	{
		current = mono_class_get_parent(current);
	}

	return current;
}

void minty::ScriptClass::invoke(String const& name) const
{
	ScriptEngine& engine = get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name);

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(nullptr, method);
}

void minty::ScriptClass::invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name, static_cast<int>(arguments.size()));

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(nullptr, method, arguments.get_values());
}

bool minty::ScriptClass::try_invoke(String const& name) const
{
	ScriptEngine& engine = get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name);

	if (!method) return false;

	// invoke it
	engine.invoke_method(nullptr, method);

	return true;
}

bool minty::ScriptClass::try_invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_class, name, static_cast<int>(arguments.size()));

	if (!method) return false;

	// invoke it
	engine.invoke_method(nullptr, method, arguments.get_values());

	return true;
}

ScriptObject minty::ScriptClass::create_object(UUID const id) const
{
	return _assembly->get_engine().create_object(*this, id);
}

bool minty::ScriptClass::has_method(String const& name) const
{
	return _assembly->get_engine().get_method(_class, name);
}

MonoType* minty::ScriptClass::get_type() const
{
	String fullName = get_full_name();
	return mono_reflection_type_from_name(fullName.data(), _assembly->_image);
}
