#include "pch.h"
#include "M_Transform.h"

using namespace minty;

minty::Transform::Transform(Vector3 const& position, Vector3 const& rotation, Vector3 const scale)
    : position(position)
    , rotation(rotation)
    , scale(scale)
{}

Matrix4 minty::Transform::get_matrix() const
{
    // combine and return
    return glm::translate(Matrix4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(Matrix4(1.0f), scale);
}

String minty::to_string(Transform const& value)
{
    return std::format("Transform(position = {}, rotation = {}, scale = {})", to_string(value.position), to_string(value.rotation), to_string(value.scale));
}
