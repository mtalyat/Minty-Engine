#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_Console.h"
#include "M_NameComponent.h"
#include <sstream>
#include <map>

using namespace minty;

std::map<std::string const, EntityRegistry::ComponentFunc const> EntityRegistry::_componentTypes = std::map<std::string const, EntityRegistry::ComponentFunc const>();

minty::EntityRegistry::EntityRegistry()
	: entt::registry()
	, Object()
{}

minty::EntityRegistry::~EntityRegistry()
{}

minty::EntityRegistry::EntityRegistry(EntityRegistry && other) noexcept
	: entt::registry(std::move(other))
{}

EntityRegistry& minty::EntityRegistry::operator=(EntityRegistry&& other) noexcept
{
	this->operator=(std::move(other));

	return *this;
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
	auto const& found = _componentTypes.find(name);
	if (found == _componentTypes.end())
	{
		// name not found
		throw std::runtime_error(std::format("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name));
	}
	else
	{
		// name found
		return found->second(this, entity);
	}
}

size_t minty::EntityRegistry::count() const
{
	return this->storage<Entity>()->size();
}

std::string const minty::EntityRegistry::to_string() const
{
	// total entities
	size_t entityCount = count();

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
