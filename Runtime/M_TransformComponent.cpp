#include "pch.h"
#include "M_TransformComponent.h"

#include "M_GLM.hpp"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

Matrix4 minty::TransformComponent::get_local_matrix() const
{
	return glm::translate(Matrix4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(Matrix4(1.0f), localScale);
}

Vector3 minty::TransformComponent::get_global_position() const
{
	return matrix4_get_position(globalMatrix);
}

Quaternion minty::TransformComponent::get_global_rotation() const
{
	return matrix4_get_rotation(globalMatrix);
}

Vector3 minty::TransformComponent::get_global_scale() const
{
	return matrix4_get_scale(globalMatrix);
}

Vector3 minty::TransformComponent::get_forward() const
{
	return Vector3(globalMatrix * Vector4(0.0f, 0.0f, 1.0f, 0.0f));
}

Vector3 minty::TransformComponent::get_up() const
{
	return Vector3(globalMatrix * Vector4(0.0f, 1.0f, 0.0f, 0.0f));
}

Vector3 minty::TransformComponent::get_right() const
{
	return Vector3(globalMatrix * Vector4(1.0f, 0.0f, 0.0f, 0.0f));
}

void minty::TransformComponent::serialize(Writer& writer) const
{
	writer.write("position", localPosition);
	writer.write("rotation", localRotation);
	writer.write("scale", localScale);
}

void minty::TransformComponent::deserialize(Reader const& reader)
{
	reader.try_read_object("position", localPosition);
	reader.try_read_object("rotation", localRotation);
	reader.try_read_object("scale", localScale);
}

String minty::to_string(TransformComponent const& value)
{
	return std::format("TransformComponent(position = {}, rotation = {}, scale = {})", to_string(value.localPosition), to_string(value.localRotation), to_string(value.localScale));
}
