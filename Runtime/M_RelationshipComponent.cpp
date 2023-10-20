#include "pch.h"
#include "M_RelationshipComponent.h"

//minty::RelationshipComponent::RelationshipComponent(size_t const children, Entity const first, Entity const prev, Entity const next, Entity const parent)
//	: children(children)
//	, first(first)
//	, prev(prev)
//	, next(next)
//	, parent(parent)
//{}

void minty::RelationshipComponent::serialize(Writer& writer) const
{

}

void minty::RelationshipComponent::deserialize(Reader const& reader)
{
}

std::string minty::to_string(RelationshipComponent const& value)
{
	return std::format("RelationshipComponent()");
}
