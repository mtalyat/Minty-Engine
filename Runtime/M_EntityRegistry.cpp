#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_Console.h"
#include "M_NameComponent.h"
#include "M_OriginComponent.h"
#include "M_PositionComponent.h"
#include "M_RotationComponent.h"
#include "M_ScaleComponent.h"
#include <sstream>
#include <map>

using namespace minty;

std::map<std::string const, EntityRegistry::ComponentFuncs const> EntityRegistry::_components = std::map<std::string const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, std::string const> EntityRegistry::_componentTypes = std::map<uint32_t const, std::string const>();

minty::EntityRegistry::EntityRegistry()
	: entt::registry()
	, Object()
{}

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

Entity minty::EntityRegistry::create(std::string const& name)
{
	Entity e = entt::registry::create();
	NameComponent& nameComponent = this->emplace<NameComponent>(e);
	nameComponent.name = name;
	return e;
}

Entity minty::EntityRegistry::find_by_name(std::string const& string) const
{
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

std::string minty::EntityRegistry::get_name(Entity const entity) const
{
	// if entity is null, return NULL
	if (entity == NULL_ENTITY)
	{
		return "NULL";
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

Component* minty::EntityRegistry::emplace_by_name(std::string const& name, Entity const entity)
{
	auto const& found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		console::error(std::format("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.create(this, entity);
	}
}

Component const* minty::EntityRegistry::get_by_name(std::string const& name, Entity const entity) const
{
	auto const& found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		console::error(std::format("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.get(this, entity);
	}
}

void minty::EntityRegistry::get_transform(Entity const entity, Transform& transform) const
{
	// get origin
	OriginComponent const* const origin = this->try_get<OriginComponent>(entity);
	if (origin)
	{
		// origin given
		transform.position = origin->position;
	}
	else
	{
		// origin is at 0, 0, 0
		transform.position = Vector3();
	}

	// get and add position to origin
	PositionComponent const* const position = this->try_get<PositionComponent>(entity);
	if (position)
	{
		transform.position += position->position;
	}

	// get rotation
	RotationComponent const* const rotation = this->try_get<RotationComponent>(entity);
	if (rotation)
	{
		transform.rotation = rotation->rotation;
	}
	else
	{
		transform.rotation = Quaternion();
	}

	// get scale
	ScaleComponent const* const scale = this->try_get<ScaleComponent>(entity);
	if (scale)
	{
		transform.scale = scale->scale;
	}
	else
	{
		transform.scale = Vector3(1.0f, 1.0f, 1.0f);
	}
}

void minty::EntityRegistry::print(Entity const entity) const
{
	// serialize entity, add it to parent, print that
	SerializedNode root;
	root.children.emplace(this->get_name(entity), serialize_entity(entity));
	root.print();
}

size_t minty::EntityRegistry::size() const
{
	return this->storage<Entity>()->size();
}

std::string const minty::EntityRegistry::to_string() const
{
	// total entities
	size_t entityCount = size();

	// named entities count
	size_t namedCount = this->view<NameComponent const>().size();

	// get a count of similarly named entities, so there isn't 100 of the same named entity
	std::map<std::string, size_t> counts;

	if (entityCount != namedCount)
	{
		// add unnamed
		counts.emplace("_", entityCount - namedCount);
	}

	if (namedCount > 0)
	{
		// add named
		for (auto&& [entity, name] : view<NameComponent const>().each())
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

void minty::EntityRegistry::serialize(Writer& writer) const
{
	// write each entity, and each component under it
	std::string entityName;

	for (auto [entity] : this->storage<Entity>()->each())
	{
		// serialize entity
		SerializedNode entityNode = serialize_entity(entity);

		// get entity name
		entityName = this->get_name(entity);

		// write entity node
		writer.write(entityName, entityNode);
	}
}

void minty::EntityRegistry::deserialize(Reader const& reader)
{
	// read each entity, add name if it has one
	
	SerializedNode const* node = reader.get_node();

	for (auto const& pair : node->children)
	{
		// create entity
		Entity const entity = entt::registry::create();

		// add name if there is one
		// empty name is either "" or "_"
		if (pair.first.size() > 1 || (pair.first.size() == 1 && pair.first.at(0) != '_'))
		{
			NameComponent& name = this->emplace<NameComponent>(entity);
			name.name = pair.first;
		}

		// now add the other components (children)
		for (auto& componentPair : pair.second.children)
		{
			Component* component = this->emplace_by_name(componentPair.first, entity);
			Reader reader(componentPair.second);
			component->deserialize(reader);
		}
	}
}

SerializedNode minty::EntityRegistry::serialize_entity(Entity const entity) const
{
	// populate a node with children
	SerializedNode entityNode;
	Writer entityWriter(entityNode);

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto const& found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
			{
				console::error(std::format("Cannot find component type with id: {}, name: {}", ctype.index(), ctype.name().data()));
				continue;
			}

			std::string name = found->second;

			// ignore NameComponent, that is used when writing the entity node
			if (name.compare("Name") == 0)
			{
				continue;
			}

			// write component with its name and serialized values
			entityWriter.write(name, this->get_by_name(name, entity));
		}
	}
	
	return entityNode;
}
