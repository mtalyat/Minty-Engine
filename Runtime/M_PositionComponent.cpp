#include "pch.h"
#include "M_PositionComponent.h"

void minty::PositionComponent::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("z", z);
}

void minty::PositionComponent::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
}
