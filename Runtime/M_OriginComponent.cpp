#include "pch.h"
#include "M_OriginComponent.h"

using namespace minty;

void minty::OriginComponent::serialize(Writer& writer) const
{
	writer.write("x", position.x, 0.0f);
	writer.write("y", position.y, 0.0f);
	writer.write("z", position.z, 0.0f);
}

void minty::OriginComponent::deserialize(Reader const& reader)
{
	position.x = reader.read_float("x");
	position.y = reader.read_float("y");
	position.z = reader.read_float("z");
}

std::string minty::to_string(OriginComponent const& value)
{
	return std::format("OriginComponent(position = {})", to_string(value.position));
}
