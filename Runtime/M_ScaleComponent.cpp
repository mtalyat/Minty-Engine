#include "pch.h"
#include "M_ScaleComponent.h"

void minty::ScaleComponent::serialize(Writer& writer) const
{
	writer.write("x", scale.x, 1.0f);
	writer.write("y", scale.y, 1.0f);
	writer.write("z", scale.z, 1.0f);
}

void minty::ScaleComponent::deserialize(Reader const& reader)
{
	scale.x = reader.read_float("x", 1.0f);
	scale.y = reader.read_float("y", 1.0f);
	scale.z = reader.read_float("z", 1.0f);
}

std::string minty::to_string(ScaleComponent const& value)
{
	return std::format("ScaleComponent(scale = {})", to_string(value.scale));
}
