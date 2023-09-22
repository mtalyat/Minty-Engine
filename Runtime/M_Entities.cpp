#include "pch.h"
#include "M_Entities.h"

#include "M_NameComponent.h"

using namespace minty;

minty::Registry::Registry()
	: entt::registry()
{
}

std::string minty::Registry::get_name(Entity const entity) const
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
