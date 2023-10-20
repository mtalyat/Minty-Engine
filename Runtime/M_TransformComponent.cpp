#include "pch.h"
#include "M_TransformComponent.h"

using namespace minty;

void minty::TransformComponent::serialize(Writer& writer) const
{
	writer.write("position", local.position, Vector3());
	writer.write("rotation", local.rotation, Quaternion());
	writer.write("scale", local.scale, Vector3(1.0f, 1.0f, 1.0f));
}

void minty::TransformComponent::deserialize(Reader const& reader)
{
	local.position = reader.read_vector3("position");
	local.rotation = reader.read_quaternion("rotation");
	local.scale = reader.read_vector3("scale", Vector3(1.0f, 1.0f, 1.0f));
}

std::string minty::to_string(TransformComponent const& value)
{
	return std::format("TransformComponent(local = {})", to_string(value.local));
}
