#include "pch.h"
#include "M_RotationComponent.h"

void minty::RotationComponent::serialize(Writer& writer) const
{
	writer.write("x", rotation.x, 0.0f);
	writer.write("y", rotation.y, 0.0f);
	writer.write("z", rotation.z, 0.0f);
}

void minty::RotationComponent::deserialize(Reader const& reader)
{
	rotation.x = reader.read_float("x");
	rotation.y = reader.read_float("y");
	rotation.z = reader.read_float("z");
}

std::string minty::to_string(RotationComponent const& value)
{
	return std::format("RotationComponent(rotation = {})", to_string(value.rotation));
}
