#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_SerializationData.h"

#include "M_Console.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_TransformComponent.h"
#include "M_DirtyComponent.h"
#include <sstream>
#include <map>

using namespace minty;

std::map<String const, EntityRegistry::ComponentFuncs const> EntityRegistry::_components = std::map<String const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, String const> EntityRegistry::_componentTypes = std::map<uint32_t const, String const>();

minty::EntityRegistry::EntityRegistry()
	: entt::registry()
	, Object()
{
	// make it so whenever a transform is editied, it is marked as dirty
	on_construct<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_update<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
}

minty::EntityRegistry::~EntityRegistry()
{}

minty::EntityRegistry::EntityRegistry(EntityRegistry&& other) noexcept
	: entt::registry(std::move(other))
{}

EntityRegistry& minty::EntityRegistry::operator=(EntityRegistry&& other) noexcept
{
	entt::registry::operator=(std::move(other));

	return *this;
}

Entity minty::EntityRegistry::create()
{
	return entt::registry::create();
}

Entity minty::EntityRegistry::create(String const& name)
{
	Entity e = create();
	NameComponent& nameComponent = this->emplace<NameComponent>(e);
	nameComponent.name = name;
	return e;
}

Entity minty::EntityRegistry::find(String const& string) const
{
	if (is_name_empty(string))
	{
		return NULL_ENTITY;
	}

	for (auto [entity, name] : this->view<NameComponent const>().each())
	{
		if (name.name.compare(string) == 0)
		{
			// found entity with same name
			return entity;
		}
	}

	// did not find entity
	return NULL_ENTITY;
}

String minty::EntityRegistry::get_name(Entity const entity) const
{
	// if entity is null, return NULL
	if (entity == NULL_ENTITY)
	{
		return "";
	}

	// if entity has a name component, get the name
	// otherwise return empty
	NameComponent const* name = try_get<NameComponent>(entity);

	if (name)
	{
		return name->name;
	}
	else
	{
		return "";
	}
}

void minty::EntityRegistry::set_name(Entity const entity, String const& name)
{
	// check if name is a real name, or empty
	if (name.empty() || (name.size() == 1 && name.front() == '_'))
	{
		// name is empty, remove the component if it exists
		if(any_of<NameComponent>(entity))
		{
			erase<NameComponent>(entity);
		}
	}
	else
	{
		// name is occupied
		NameComponent& nameComponent = get_or_emplace<NameComponent>(entity);
		nameComponent.name = name;
	}
}

Component* minty::EntityRegistry::emplace_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.emplace(*this, entity);
	}
}

Component* minty::EntityRegistry::get_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return const_cast<Component*>(static_cast<Component const*>(found->second.get(*this, entity)));
	}
}

Component const* minty::EntityRegistry::get_by_name(String const& name, Entity const entity) const
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.get(*this, entity);
	}
}

std::vector<Component const*> minty::EntityRegistry::get_all(Entity const entity) const
{
	std::vector<Component const*> components;

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
			{
				Console::error(std::format("Cannot find_animation component type with id: {}, name: {}", ctype.index(), ctype.name().data()));
				continue;
			}

			String name = found->second;

			components.push_back(this->get_by_name(found->second, entity));
		}
	}

	return components;
}

void minty::EntityRegistry::erase_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot erase Component \"{}\". It has not been registered with the EntityRegistry.", name));
	}
	else
	{
		// name found
		found->second.erase(*this, entity);
	}
}

Entity minty::EntityRegistry::clone(Entity const entity)
{
	Entity newEntity = create();

	for (auto [id, storage] : this->storage()) {
		if (storage.contains(entity) && !storage.contains(newEntity)) {
			if (void* value = storage.value(entity))
			{
				storage.push(newEntity, value);
			}
		}
	}

	return newEntity;
}

void minty::EntityRegistry::print(Entity const entity) const
{
	// serialize entity, add it to parent, print that
	Console::print(serialize_entity(entity));
}

size_t minty::EntityRegistry::size() const
{
	return this->storage<Entity>()->size();
}

void minty::EntityRegistry::serialize(Writer& writer) const
{
	// write each entity, and each component under it
	String entityName;

	for (auto [entity] : this->storage<Entity>()->each())
	{
		// get name
		entityName = this->get_name(entity);

		// use "-" instead of ""
		if (entityName.empty()) entityName = "-";

		// serialize entity
		Node entityNode(entityName);
		Writer entityWriter(entityNode, writer.get_data());

		serialize_entity(entityWriter, entity);

		// write entity node
		writer.write(entityNode);
	}
}

void minty::EntityRegistry::deserialize(Reader const& reader)
{
	// read each entity, add name if it has one
	
	Node const& node = reader.get_node();
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	// for each entity name given
	for (auto const& entityNode : node.get_children())
	{
		// create the entity
		Entity entity = create();
		data.entity = entity;

		// set name
		set_name(entity, entityNode.get_name());

		// cycle through each component on entity
		for (auto const& compNode : entityNode.get_children())
		{
			// cannot have multiple of same component
			if (get_by_name(compNode.get_name(), entity))
			{
				Console::error(std::format("Attempting to deserialize entity \"{}\" with multiple instances of the \"{}\" component.", entityNode.get_name(), compNode.get_name()));
				return;
			}

			// add component
			Component* comp = emplace_by_name(compNode.get_name(), entity);
			Reader compReader(compNode, &data);
			comp->deserialize(compReader);
		}
	}
}

bool minty::EntityRegistry::is_name_empty(String const& name)
{
	return name.size() == 0 || (name.size() == 1 && name.at(0) == '_');
}

void minty::EntityRegistry::serialize_entity(Writer& writer, Entity const entity) const
{
	SerializationData data = *static_cast<SerializationData const*>(writer.get_data());
	data.entity = entity;
	Writer tempWriter(writer.get_node(), &data);

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
			{
				Console::error(std::format("Cannot find_animation component type with id: {}, name: {}", ctype.index(), ctype.name().data()));
				continue;
			}

			String name = found->second;

			// ignore NameComponent, that is used when writing the entity node
			if (name.compare("Name") == 0)
			{
				continue;
			}

			// write component with its name and serialized values
			tempWriter.write(name, this->get_by_name(name, entity));
		}
	}
}

Node minty::EntityRegistry::serialize_entity(Entity const entity) const
{
	Node node;
	Writer writer(node);
	serialize_entity(writer, entity);
	return node;
}

String minty::to_string(Entity const value)
{
	return std::to_string(static_cast<unsigned int>(value));
}

String minty::to_string(EntityRegistry const& value)
{
	// total entities
	size_t entityCount = value.size();

	// named entities count
	size_t namedCount = value.view<NameComponent const>().size();

	// get a count of similarly named entities, so there isn't 100 of the same named entity
	std::map<String, size_t> counts;

	if (entityCount != namedCount)
	{
		// add unnamed
		counts.emplace("_", entityCount - namedCount);
	}

	if (namedCount > 0)
	{
		// add named
		for (auto&& [entity, name] : value.view<NameComponent const>().each())
		{
			if (counts.find(name.name) != counts.end())
			{
				// name exists
				counts[name.name] += 1;
			}
			else
			{
				// name does not exist yet
				counts[name.name] = 1;
			}
		}
	}

	// build string list

	std::stringstream stream;

	stream << "EntityRegistry(";

	size_t i = 0;
	for (auto const& pair : counts)
	{
		if (i > 0)
		{
			stream << ", ";
		}

		// add name
		stream << pair.first;

		// add number if more than one
		if (pair.second != 1)
		{
			stream << " (x" << pair.second << ")";
		}

		i++;
	}

	stream << ")";

	return stream.str();
}
