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
    //Matrix4 rotationMatrix = glm::rotate(Matrix4(1.0f), rotation.y, Vector3(0.0f, 1.0f, 0.0f));
    //rotationMatrix = glm::rotate(rotationMatrix, rotation.x, Vector3(1.0f, 0.0f, 0.0f));
    //rotationMatrix = glm::rotate(rotationMatrix, rotation.z, Vector3(0.0f, 0.0f, 1.0f));
    Matrix4 rotationMatrix = glm::mat4_cast(rotation);
    Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), scale);

    // combine and return
    return translationMatrix * rotationMatrix * scaleMatrix;
}
