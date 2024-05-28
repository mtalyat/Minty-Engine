#include "pch.h"
#include "Minty/Scripting/M_ScriptObject.h"

#include "Minty/Libraries/M_Mono.h"

#include "Minty/Scripting/M_ScriptClass.h"
#include "Minty/Scripting/M_ScriptArguments.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"

using namespace Minty;
using namespace Minty::Scripting;

Minty::ScriptObject::ScriptObject(UUID const id, ScriptClass const& script)
	: ScriptObjectComponent(id)
	, _script(&script)
	, _object(ScriptEngine::instance().create_instance(script._class))
	, _handle(mono_gchandle_new(_object, false))
{}

Minty::ScriptObject::ScriptObject(UUID const id, ScriptClass const& script, ScriptArguments& arguments)
	: ScriptObjectComponent(id)
	, _script(&script)
	, _object(ScriptEngine::instance().create_instance(script._class, arguments.get_values()))
	, _handle(mono_gchandle_new(_object, false))
{}

Minty::ScriptObject::~ScriptObject()
{}

void Minty::ScriptObject::destroy()
{
	_script = nullptr;
	_object = nullptr;
	mono_gchandle_free(_handle);
	_handle = 0;
}

void Minty::ScriptObject::invoke(String const& name) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name);

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(data(), method);
}

void Minty::ScriptObject::invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name, static_cast<int>(arguments.size()));

	MINTY_ASSERT(method != nullptr);

	// invoke it
	engine.invoke_method(data(), method, arguments.get_values());
}

bool Minty::ScriptObject::try_invoke(String const& name) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name);

	if (method == nullptr) return false;

	// invoke it
	engine.invoke_method(data(), method);

	return true;
}

bool Minty::ScriptObject::try_invoke(String const& name, ScriptArguments& arguments) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find the method to invoke
	MonoMethod* method = engine.get_method(_script->_class, name, static_cast<int>(arguments.size()));

	if (method == nullptr) return false;

	// invoke it
	engine.invoke_method(data(), method, arguments.get_values());

	return true;
}

ScriptClass const& Minty::ScriptObject::get_class() const
{
	return *_script;
}

void Minty::ScriptObject::set_field(String const& name, void* const value) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	engine.set_field_value(data(), engine.get_field(_script->_class, name), value);
}

void Minty::ScriptObject::get_field(String const& name, void* const value) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	engine.get_field_value(data(), engine.get_field(_script->_class, name), value);
}

void Minty::ScriptObject::set_property(String const& name, void* const value) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	engine.set_property_value(data(), engine.get_property(_script->_class, name), value);
}

void Minty::ScriptObject::get_property(String const& name, void* const value) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	engine.get_property_value(data(), engine.get_property(_script->_class, name), value);
}

MonoObject* Minty::ScriptObject::data() const
{
	return _object;
	//return mono_gchandle_get_target(_handle);
}

void Minty::ScriptObject::serialize(Writer& writer) const
{
	ScriptEngine& engine = ScriptEngine::instance();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->_class, Accessibility::Public);

	MonoObject* object = data();

	// write each name and value
	for (MonoClassField* const field : publicFields)
	{
		serialize_field(writer, engine.get_field_name(field), field, object);
	}
}

void Minty::ScriptObject::deserialize(Reader const& reader)
{
	ScriptEngine& engine = ScriptEngine::instance();

	// use the assembly to retrieve all of the fields for this object
	std::vector<MonoClassField*> publicFields = engine.get_fields(_script->_class, Accessibility::Public);

	MonoObject* object = data();

	for (MonoClassField* const field : publicFields)
	{
		deserialize_field(reader, engine.get_field_name(field), field, object);
	}
}

void Minty::ScriptObject::serialize_field(Writer& writer, String const& name, MonoClassField* const field, MonoObject* const object) const
{
	ScriptEngine& engine = ScriptEngine::instance();

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

void Minty::ScriptObject::deserialize_field(Reader const& reader, String const& name, MonoClassField* const field, MonoObject* const object)
{
	ScriptEngine& engine = ScriptEngine::instance();

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
