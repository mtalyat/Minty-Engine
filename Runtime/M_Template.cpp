#include "pch.h"
#include "M_Template.h"

#include "M_RelationshipComponent.h"
#include "M_Register.h"

using namespace minty;

minty::Template::Template()
{
}

minty::Template::Template(Entity const entity, EntityRegistry& registry)
{
	std::unordered_map<Entity, ID> entities;

	// add root
	ID rootId = _data.emplace(Data());
	entities.emplace(entity, rootId);

	// only add children for root entity
	if (RelationshipComponent* relationship = registry.try_get<RelationshipComponent>(entity))
	{
		
	}
}

void minty::Template::emplace(Entity const entity, EntityRegistry& registry, std::unordered_map<Entity, ID>& entities)
{
	// recursively add hierarchy
	if (RelationshipComponent* relationship = registry.try_get<RelationshipComponent>(entity))
	{
		// add all children and siblings
	}
}

void minty::Template::serialize(Writer& writer) const
{

}

void minty::Template::deserialize(Reader const& reader)
{

}
