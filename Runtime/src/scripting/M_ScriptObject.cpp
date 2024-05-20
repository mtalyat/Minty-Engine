#include "pch.h"
#include "scripting/M_ScriptObject.h"

#include "libraries/M_Mono.h"

#include "scripting/M_ScriptClass.h"
#include "scripting/M_ScriptArguments.h"
#include "scripting/M_ScriptEngine.h"
#include "entities/M_EntityRegistry.h"
#include "tools/M_Console.h"
#include "serialization/M_Writer.h"
#include "serialization/M_Reader.h"

using namespace minty;
using namespace minty::Scripting;

minty::ScriptObject::ScriptObject(UUID const id, ScriptClass const& script)
	: Component(id)
	, _script(&script)
	, _object(script.get_assembly().get_engine().create_instance(script._class))
	, _handle(mono_gchandle_new(_object, false))
{}

minty::ScriptObject::ScriptObject(UUID const id, ScriptClass const& script, ScriptArguments& arguments)
	: Component(id)
	, _script(&script)
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

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(data(), method);
}

void minty::ScriptObject::invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name, static_cast<int>(arguments.size()));

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(data(), method, arguments.get_values());
}

bool minty::ScriptObject::try_invoke(String const& name) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name);

	if (method == nullptr) return false;

	// invoke it
	engine.invoke_method(data(), method);

	return true;
}

bool minty::ScriptObject::try_invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name, static_cast<int>(arguments.size()));

	if (method == nullptr) return false;

	// invoke it
	engine.invoke_method(data(), method, arguments.get_values());

	return true;
}

ScriptClass const& minty::ScriptObject::get_class() const
{
	return *_script;
}

void minty::ScriptObject::set_field(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.set_field_value(data(), engine.get_field(_script->_class, name), value);
}

void minty::ScriptObject::get_field(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.get_field_value(data(), engine.get_field(_script->_class, name), value);
}

void minty::ScriptObject::set_property(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.set_property_value(data(), engine.get_property(_script->_class, name), value);
}

void minty::ScriptObject::get_property(String const& name, void* const value) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	engine.get_property_value(data(), engine.get_property(_script->_class, name), value);
}

MonoObject* minty::ScriptObject::data() const
{
	return _object;
	//return mono_gchandle_get_target(_handle);
}

void minty::ScriptObject::serialize(Writer& writer) const
{
	ScriptEngine& engine = _script->get_assembly().get_engine();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->_class, Accessibility::Public);

	MonoObject* object = data();

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

	MonoObject* object = data();

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
		MINTY_TODO_FORMAT("ScriptObject::serialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum));
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
		MINTY_TODO_FORMAT("ScriptObject::deserialize_field(): missing type {}.", static_cast<int>(fieldTypeEnum));
	}
}
