#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_Console.h"
#include "M_NameComponent.h"

using namespace minty;

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

void minty::EntityRegistry::register_component(std::string const& name, ComponentFunc const& func)
{
	_componentTypes.emplace(name, func);
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
		return found->second(*this, entity);
	}
}
