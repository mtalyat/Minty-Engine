#include "pch.h"
#include "M_PositionComponent.h"

void minty::PositionComponent::serialize(Writer& writer) const
{
	writer.write("x", position.x, 0.0f);
	writer.write("y", position.y, 0.0f);
	writer.write("z", position.z, 0.0f);
}

void minty::PositionComponent::deserialize(Reader const& reader)
{
	position.x = reader.read_float("x");
	position.y = reader.read_float("y");
	position.z = reader.read_float("z");
}
