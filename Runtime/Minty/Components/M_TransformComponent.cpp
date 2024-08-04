#include "pch.h"
#include "Minty/Components/M_TransformComponent.h"

#include "Minty/Libraries/M_GLM.hpp"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

Matrix4 Minty::TransformComponent::get_local_matrix() const
{
	return glm::translate(Matrix4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(Matrix4(1.0f), localScale);
}

Vector3 Minty::TransformComponent::get_global_position() const
{
	return matrix4_get_position(globalMatrix);
}

Quaternion Minty::TransformComponent::get_global_rotation() const
{
	return matrix4_get_rotation(globalMatrix);
}

Vector3 Minty::TransformComponent::get_global_scale() const
{
	return matrix4_get_scale(globalMatrix);
}

Vector3 Minty::TransformComponent::get_forward() const
{
	return Vector3(globalMatrix * Vector4(0.0f, 0.0f, 1.0f, 0.0f));
}

Vector3 Minty::TransformComponent::get_up() const
{
	return Vector3(globalMatrix * Vector4(0.0f, 1.0f, 0.0f, 0.0f));
}

Vector3 Minty::TransformComponent::get_right() const
{
	return Vector3(globalMatrix * Vector4(1.0f, 0.0f, 0.0f, 0.0f));
}

void Minty::TransformComponent::serialize(Writer& writer) const
{
	writer.write("position", localPosition);
	writer.write("rotation", localRotation);
	writer.write("scale", localScale);
}

void Minty::TransformComponent::deserialize(Reader const& reader)
{
	reader.try_read_object("position", localPosition);
	reader.try_read_object("rotation", localRotation);
	reader.try_read_object("scale", localScale);
}

TransformComponent Minty::TransformComponent::create_empty()
{
	TransformComponent component = {};
	component.globalMatrix = component.get_local_matrix();
	return component;
}
