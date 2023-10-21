#include "pch.h"
#include "M_RenderableComponent.h"

using namespace minty;

void minty::RenderableComponent::serialize(Writer& writer) const
{}

void minty::RenderableComponent::deserialize(Reader const& reader)
{}

std::string minty::to_string(RenderableComponent const& value)
{
	return std::format("RenderableComponent()");
}
