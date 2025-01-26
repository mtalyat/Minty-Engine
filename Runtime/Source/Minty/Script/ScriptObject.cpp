#include "pch.h"
#include "ScriptObject.h"

#include "Minty/Core/Application.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Script/CS/CsScriptObject.h"
#include "Minty/Script/ScriptClass.h"
#include "Minty/Script/ScriptEngine.h"

using namespace Minty;

Minty::ScriptObject::ScriptObject(ScriptObjectBuilder const& builder)
	: m_class(builder.klass)
	, m_fields()
	, m_properties()
	, m_methods()
{ }

Owner<ScriptObject> Minty::ScriptObject::create(ScriptObjectBuilder const& builder)
{
	Owner<ScriptObject> obj = Owner<CsScriptObject>(builder);
	obj->populate();
	return obj;
}

Owner<ScriptObject> Minty::ScriptObject::create(ScriptObjectBuilder const& builder, ScriptArguments& args)
{
	Owner<ScriptObject> obj = Owner<CsScriptObject>(builder, args);
	obj->populate();
	return obj;
}

void Minty::ScriptObject::serialize(Writer& writer) const
{
	EntityRegistry& entityRegistry = Application::instance().get_scene_manager().get_working_scene()->get_entity_registry();

	// write all fields
	for (auto const& [name, field] : m_fields)
	{
		// only show public fields
		if (field->get_accessibility() != Accessibility::Public)
		{
			continue;
		}

		Type type = field->get_type();

		// if type is undefined, ignore
		if (type == Type::Undefined)
		{
			continue;
		}

		void* data = nullptr;

		switch (type)
		{
		case Type::Object:
		{
			// get object
			field->get(&data);

			// get ID from object
			UUID id{};
			if (data != nullptr)
			{
				id = ScriptEngine::get_entity_uuid(data);

				MINTY_ASSERT_FORMAT(entityRegistry.find_by_id(id) != NULL_ENTITY || AssetManager::contains(id), "Failed to serialize ScriptObject. Invalid ID: {}. ID must belong to a valid Entity or Asset.", to_string(id));
			}

			// write id
			writer.write(name, &id, Type::UUID);
			break;
		}
		default:
		{
			// get value
			data = new Byte[sizeof_type(type) << 1]; // TODO: WHY does this need double space (for ulong specifically)
			field->get(data);

			// write value
			writer.write(name, data, type);

			// clean up
			delete[] data;
			break;
		}
		}
	}
}

void Minty::ScriptObject::deserialize(Reader& reader)
{
	Ref<ScriptAssembly> mintyAssembly = ScriptEngine::get_assembly(MINTY_NAME_ENGINE);

	// read all fields, if able
	for (auto const& [name, field] : m_fields)
	{
		Type type = field->get_type();

		// if type is undefined, ignore
		if (type == Type::Undefined)
		{
			continue;
		}

		Type readType;

		// if type is object, read as UUID
		if (type == Type::Object)
		{
			readType = Type::UUID;
		}
		else
		{
			readType = type;
		}

		Byte* data = new Byte[sizeof_type(readType)];
		if (reader.read(name, data, readType))
		{
			switch (type)
			{
			case Type::Object:
			{
				// get ID
				UUID id{};
				memcpy(&id, data, sizeof(UUID));

				// get entity with ID
				void* object = nullptr;
				if (id.valid())
				{
					// TODO: make nicer, check(s) for null
					
					// if asset, get the asset
					// if entity, get the entity, or the appropriate component attached to it
					if (AssetManager::contains(id))
					{
						// asset
						String fieldName = field->get_type_full_name();
						Ref<ScriptClass> scriptClass = mintyAssembly->get_class(fieldName);
						Ref<ScriptObject> scriptObject = ScriptEngine::get_or_create_object_asset(id, scriptClass);
						object = scriptObject->get_native();
					}
					else
					{
						// not an asset
						
						// get registry and assembly
						EntityRegistry& entityRegistry = Application::instance().get_scene_manager().get_working_scene()->get_entity_registry();
						String scriptClassName = field->get_type_full_name();

						// get the entity with the given ID
						Entity entity = entityRegistry.find_by_id(id);
						MINTY_ASSERT_FORMAT(entity != NULL_ENTITY, "Entity does not exist with an ID of {}.", to_string(id));

						Ref<ScriptObject> entityObject = ScriptEngine::get_or_create_object_entity(id);

						// if looking for an Entity, get that object, otherwise get the Component attached to this Entity
						Ref<ScriptObject> scriptObject;
						String fieldTypeName = field->get_type_full_name();
						if (fieldTypeName == "MintyEngine.Entity")
						{
							// Entity
							scriptObject = entityObject;
							object = scriptObject->get_native();
						}
						else
						{
							// Component
							MINTY_LOG("Still need to add how to serialize Components within a ScriptObject. Sorry! :(");
							MINTY_NOT_IMPLEMENTED();
							//String name = fieldTypeName;
							//if (name.starts_with("MintyEngine."))
							//{
							//	name = name.substr(12, name.length() - 12);
							//}

							//ScriptObjectComponent* component = static_cast<ScriptObjectComponent*>(entityRegistry.get_by_name(name, entity));
							//Ref<ScriptClass> scriptClass = ScriptEngine::find_class(fieldTypeName);

							//// if no ID, create a new object
							//if (!component->get_id().valid())
							//{
							//	component->set_id(UUID::create());
							//}

							//scriptObject = ScriptEngine::get_or_create_object_component(component->get_id(), id, scriptClass);
							//object = scriptObject->get_native();
						}
					}
				}
				else
				{
					object = nullptr;
				}

				field->set(object);
				break;
			}
			default:
			{
				field->set(data);
				break;
			}
			}
		}
		delete[] data;
	}
}

Ref<ScriptField> Minty::ScriptObject::get_field(String const& name) const
{
	auto found = m_fields.find(name);

	if (found == m_fields.end())
	{
		return Ref<ScriptField>();
	}

	return found->second.create_ref();
}

std::vector<Ref<ScriptField>> Minty::ScriptObject::get_fields() const
{
	std::vector<Ref<ScriptField>> fields;
	fields.reserve(m_fields.size());

	for (auto const& [name, field] : m_fields)
	{
		fields.push_back(field);
	}

	return fields;
}

Ref<ScriptProperty> Minty::ScriptObject::get_property(String const& name) const
{
	auto found = m_properties.find(name);

	if (found == m_properties.end())
	{
		return Ref<ScriptProperty>();
	}

	return found->second.create_ref();
}

std::vector<Ref<ScriptProperty>> Minty::ScriptObject::get_properties() const
{
	std::vector<Ref<ScriptProperty>> properties;
	properties.reserve(m_properties.size());

	for (auto const& [name, prop] : m_properties)
	{
		properties.push_back(prop);
	}

	return properties;
}

Ref<ScriptMethod> Minty::ScriptObject::get_method(String const& name, Int const parameterCount) const
{
	auto found = m_methods.find(name);

	if (found == m_methods.end())
	{
		return Ref<ScriptMethod>();
	}

	return found->second.create_ref();
}

std::vector<Ref<ScriptMethod>> Minty::ScriptObject::get_methods() const
{
	std::vector<Ref<ScriptMethod>> methods;
	methods.reserve(m_methods.size());

	for (auto const& [name, method] : m_methods)
	{
		methods.push_back(method);
	}

	return methods;
}

void Minty::ScriptObject::invoke(String const& name)
{
	Ref<ScriptMethod> method = get_method(name, 0);

	MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a method named \"{}\".", get_class()->get_full_name(), name);

	method->invoke();
}

void Minty::ScriptObject::invoke(String const& name, void** const argv, Size const argc)
{
	Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

	MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a method named \"{}\".", get_class()->get_full_name(), name);

	method->invoke(argv, argc);
}

Bool Minty::ScriptObject::try_invoke(String const& name)
{
	Ref<ScriptMethod> method = get_method(name, 0);

	if (method == nullptr)
	{
		return false;
	}

	method->invoke();

	return true;
}

Bool Minty::ScriptObject::try_invoke(String const& name, void** const argv, Size const argc)
{
	Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

	if (method == nullptr)
	{
		return false;
	}

	method->invoke(argv, argc);

	return true;
}

void Minty::ScriptObject::set_field(String const& name, void* const value)
{
	Ref<ScriptField> field = get_field(name);

	MINTY_ASSERT_FORMAT(field != nullptr, "{} does not contain a field named \"{}\".", get_class()->get_full_name(), name);

	field->set(value);
}

void Minty::ScriptObject::get_field(String const& name, void* const value)
{
	Ref<ScriptField> field = get_field(name);

	MINTY_ASSERT_FORMAT(field != nullptr, "{} does not contain a field named \"{}\".", get_class()->get_full_name(), name);

	field->get(value);
}

void Minty::ScriptObject::set_property(String const& name, void** const value)
{
	Ref<ScriptProperty> prop = get_property(name);

	MINTY_ASSERT_FORMAT(prop != nullptr, "{} does not contain a property named \"{}\".", get_class()->get_full_name(), name);

	prop->set(value);
}

void Minty::ScriptObject::get_property(String const& name, void** const value)
{
	Ref<ScriptProperty> prop = get_property(name);

	MINTY_ASSERT_FORMAT(prop != nullptr, "{} does not contain a property named \"{}\".", get_class()->get_full_name(), name);

	prop->set(value);
}

void Minty::ScriptObject::set_id(UUID const id)
{
	// set ID on ScriptObject, and locally
	m_id = id;
	UUID_t idRaw = id.data();
	set_field("ID", &idRaw);
}

void Minty::ScriptObject::populate()
{
	m_fields.clear();
	std::vector<Owner<ScriptField>> fields;
	this->populate_fields(fields);
	m_fields.reserve(fields.size());
	for (auto const& field : fields)
	{
		m_fields.emplace(field->get_name(), field);
	}

	m_properties.clear();
	std::vector<Owner<ScriptProperty>> properties;
	this->populate_properties(properties);
	m_properties.reserve(properties.size());
	for (auto const& prop : properties)
	{
		m_properties.emplace(prop->get_name(), prop);
	}

	m_methods.clear();
	std::vector<Owner<ScriptMethod>> methods;
	this->populate_methods(methods);
	m_methods.reserve(methods.size());
	for (auto const& method : methods)
	{
		m_methods.emplace(method->get_name(), method);
	}
}
