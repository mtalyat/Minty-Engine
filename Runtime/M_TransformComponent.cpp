#include "pch.h"
#include "M_TransformComponent.h"

using namespace minty;

Vector3 minty::TransformComponent::get_global_position() const
{
	// last column of the matrix is the position
	return Vector3(global[3]);
}

Vector3 minty::TransformComponent::get_forward() const
{
	return Vector3(global * Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

Vector3 minty::TransformComponent::get_up() const
{
	return Vector3(global * Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}

Vector3 minty::TransformComponent::get_right() const
{
	return Vector3(global * Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

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

String minty::to_string(TransformComponent const& value)
{
	return std::format("TransformComponent(local = {})", to_string(value.local));
}
