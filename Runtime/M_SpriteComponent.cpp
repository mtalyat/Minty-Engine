#include "pch.h"
#include "M_SpriteComponent.h"

using namespace minty;

void minty::SpriteComponent::serialize(Writer& writer) const
{
	writer.write("id", id, 0);
	writer.write("layer", layer, 0);
}

void minty::SpriteComponent::deserialize(Reader const& reader)
{
	id = reader.read_id("id");
	layer = reader.read_int("layer");
}

std::string minty::to_string(SpriteComponent const& value)
{
	return std::format("SpriteComponent(id = {}, layer = {})", value.id, value.layer);
}
