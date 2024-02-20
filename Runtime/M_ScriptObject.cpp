#include "pch.h"
#include "M_ScriptObject.h"

#include "M_Mono.h"

#include "M_ScriptClass.h"
#include "M_ScriptArguments.h"
#include "M_ScriptEngine.h"
#include "M_EntityRegistry.h"
#include "M_Console.h"
#include "M_Writer.h"
#include "M_Reader.h"

using namespace minty;
using namespace minty::Scripting;

minty::ScriptObject::ScriptObject(ScriptClass const& script)
	: _script(&script)
	, _object(script.get_assembly().get_engine().create_instance(script._class))
	, _handle(mono_gchandle_new(_object, false))
{}

minty::ScriptObject::ScriptObject(ScriptClass const& script, ScriptArguments& arguments)
	: _script(&script)
	, _object(script.get_assembly().get_engine().create_instance(script._class, arguments.get_values()))
	, _handle(mono_gchandle_new(_object, false))
{}

minty::ScriptObject::~ScriptObject()
{}

void minty::ScriptObject::destroy()
{
	_script = nullptr;
	_object = nullptr;
	mono_gchandle_free(_handle);
	_handle = 0;
}

void minty::ScriptObject::invoke(String const& name) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name);

	// invoke it
	engine.invoke_method(get_object(), method);
}

void minty::ScriptObject::set_field(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.set_field_value(get_object(), engine.get_field(_script->_class, name), value);
}

void minty::ScriptObject::get_field(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.get_field_value(get_object(), engine.get_field(_script->_class, name), value);
}

void minty::ScriptObject::set_property(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.set_property_value(get_object(), engine.get_property(_script->_class, name), value);
}

void minty::ScriptObject::get_property(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.get_property_value(get_object(), engine.get_property(_script->_class, name), value);
}

MonoObject* minty::ScriptObject::get_object() const
{
	return _object;
	//return mono_gchandle_get_target(_handle);
}

void minty::ScriptObject::serialize(Writer& writer) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->_class, Accessibility::Public);

	MonoObject* object = get_object();

	// write each name and value
	for (MonoClassField* const field : publicFields)
	{
		serialize_field(writer, engine.get_field_name(field), field, object);
	}
}

void minty::ScriptObject::deserialize(Reader const& reader)
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->_class, Accessibility::Public);

	MonoObject* object = get_object();

	for (MonoClassField* const field : publicFields)
	{
		deserialize_field(reader, engine.get_field_name(field), field, object);
	}
}

void minty::ScriptObject::serialize_field(Writer& writer, String const& name, MonoClassField* const field, MonoObject* const object) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// get the type
	MonoType* fieldType = engine.get_field_type(field);

	// get the type enum
	MonoTypeEnum fieldTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(fieldType));

	switch (fieldTypeEnum)
	{
	case MONO_TYPE_I4: // int
	{
		int value;
		get_field(name, &value);
		engine.get_field_value(object, field, &value);
		writer.write(name, value);
		break;
	}
	case MONO_TYPE_U4: // uint
	{
		unsigned int value;
		engine.get_field_value(object, field, &value);
		writer.write(name, value);
		break;
	}
	case MONO_TYPE_R4: // float
	{
		float value;
		engine.get_field_value(object, field, &value);
		writer.write(name, value);
		break;
	}
	default:
		Console::todo(std::format("ScriptObject::serialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum)));
	}
}

void minty::ScriptObject::deserialize_field(Reader const& reader, String const& name, MonoClassField* const field, MonoObject* const object)
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// get the type
	MonoType* fieldType = engine.get_field_type(field);

	// get the type enum
	MonoTypeEnum fieldTypeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(fieldType));

	switch (fieldTypeEnum)
	{
	case MONO_TYPE_I4: // int
	{
		int value = reader.read_int(name);
		engine.set_field_value(object, field, &value);
		break;
	}
	case MONO_TYPE_U4: // uint
	{
		unsigned int value = reader.read_uint(name);
		engine.set_field_value(object, field, &value);
		break;
	}
	case MONO_TYPE_R4: // float
	{
		float value = reader.read_float(name);
		engine.set_field_value(object, field, &value);
		break;
	}
	default:
		Console::todo(std::format("ScriptObject::deserialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum)));
	}
}
