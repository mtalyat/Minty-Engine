#include "pch.h"
#include "M_RotationComponent.h"

void minty::RotationComponent::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("z", z);
}

void minty::RotationComponent::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
}