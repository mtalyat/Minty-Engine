#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_Console.h"
#include "M_NameComponent.h"

using namespace minty;

std::map<std::string const, EntityRegistry::ComponentFunc const> EntityRegistry::_componentTypes = std::map<std::string const, EntityRegistry::ComponentFunc const>();

minty::EntityRegistry::EntityRegistry()
	: entt::registry()
{
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
		console::error(std::format("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second(this, entity);
	}
}

std::string const minty::EntityRegistry::to_string() const
{
	return std::format("[{}({})]", typeid(*this).name(), storage<Entity>()->size());
}
