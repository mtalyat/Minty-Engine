#include "pch.h"
#include "M_ScriptObject.h"

#include "M_Mono.h"

#include "M_Script.h"
#include "M_ScriptEngine.h"
#include "M_Console.h"

using namespace minty;
using namespace minty::Scripting;

minty::ScriptObject::ScriptObject(Script const& script)
	: _script(&script)
	, _object(script.create_instance())
{
	invoke("OnCreate");
}

minty::ScriptObject::~ScriptObject()
{
	invoke("OnDestroy");
}

void minty::ScriptObject::invoke(String const& name) const
{
	ScriptEngine& engine = _script->get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->get_class(), name);

	// invoke it
	engine.invoke_method(_object, method);
}

void minty::ScriptObject::serialize(Writer& writer) const
{
	ScriptEngine& engine = _script->get_engine();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->get_class(), Accessibility::Public);

	// write each name and value
	for (MonoClassField* const field : publicFields)
	{
		serialize_field(writer, engine.get_field_name(field), field);
	}
}

void minty::ScriptObject::deserialize(Reader const& reader)
{
	ScriptEngine& engine = _script->get_engine();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->get_class(), Accessibility::Public);

	for (MonoClassField* const field : publicFields)
	{
		deserialize_field(reader, engine.get_field_name(field), field);
	}
}

void minty::ScriptObject::serialize_field(Writer& writer, String const& name, MonoClassField* const field) const
{
	ScriptEngine& engine = _script->get_engine();

	// get the type
	MonoType* fieldType = engine.get_field_type(field);

	// get the type enum
	MonoTypeEnum fieldTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(fieldType));

	switch (fieldTypeEnum)
	{
	case MONO_TYPE_R4: // float
	{
		float value;
		engine.get_field_value(_object, field, &value);
		writer.write(name, value);
		break;
	}
	default:
		Console::todo(std::format("ScriptObject::serialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum)));
	}
}

void minty::ScriptObject::deserialize_field(Reader const& reader, String const& name, MonoClassField* const field)
{
	ScriptEngine& engine = _script->get_engine();

	// get the type
	MonoType* fieldType = engine.get_field_type(field);

	// get the type enum
	MonoTypeEnum fieldTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(fieldType));

	switch (fieldTypeEnum)
	{
	case MONO_TYPE_R4: // float
	{
		float value = reader.read_float(name);
		engine.set_field_value(_object, field, &value);
		break;
	}
	default:
		Console::todo(std::format("ScriptObject::deserialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum)));
	}
}
