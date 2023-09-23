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
	x = reader.read_float("x", 1.0f);
	y = reader.read_float("y", 1.0f);
	z = reader.read_float("z", 1.0f);
}