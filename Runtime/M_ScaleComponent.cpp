#include "pch.h"
#include "M_ScaleComponent.h"

void minty::ScaleComponent::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("z", z);
}

void minty::ScaleComponent::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
}