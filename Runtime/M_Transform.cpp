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
    // get translation, rotation, scale matrices
    Matrix4 translationMatrix = glm::translate(Matrix4(1.0f), position);
    Matrix4 rotationMatrix = glm::mat4_cast(rotation);
    Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), scale);

    // combine and return
    return translationMatrix * rotationMatrix * scaleMatrix;
}

std::string minty::to_string(Transform const& value)
{
    return std::format("Transform(position = {}, rotation = {}, scale = {})", to_string(value.position), to_string(value.rotation), to_string(value.scale));
}
