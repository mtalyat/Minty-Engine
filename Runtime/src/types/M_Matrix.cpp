#include "pch.h"
#include "types/M_Matrix.h"

using namespace minty;

Vector3 minty::matrix4_get_position(Matrix4 const& value)
{
    return value[3];
}

Quaternion minty::matrix4_get_rotation(Matrix4 const& value)
{
    return matrix4_get_rotation(value, matrix4_get_scale(value));
}

Quaternion minty::matrix4_get_rotation(Matrix4 const& value, Vector3 const scale)
{
    Matrix3 rotationMatrix;
    rotationMatrix[0] = value[0] / scale.x;
    rotationMatrix[1] = value[1] / scale.y;
    rotationMatrix[2] = value[2] / scale.z;
    return glm::quat_cast(rotationMatrix);
}

Vector3 minty::matrix4_get_scale(Matrix4 const& value)
{
    return Vector3(glm::length(value[0]), glm::length(value[1]), glm::length(value[2]));
}

void minty::matrix4_set_position(Matrix4& value, Vector3 const position)
{
    value[3][0] = position.x;
    value[3][1] = position.y;
    value[3][2] = position.z;
}

void minty::matrix4_set_rotation(Matrix4& value, Quaternion const position)
{

}

void minty::matrix4_set_scale(Matrix4& value, Vector3 const position)
{

}

String minty::to_string(Matrix2 const& value)
{
    return std::format("Matrix2({}, {}, {}, {})", value[0][0], value[0][1], value[1][0], value[1][1]);
}

String minty::to_string(Matrix3 const& value)
{
    return std::format("Matrix3({}, {}, {}, {}, {}, {}, {}, {}, {})", value[0][0], value[0][1], value[0][2], value[1][0], value[1][1], value[1][2], value[2][0], value[2][1], value[2][2]);
}

String minty::to_string(Matrix4 const& value)
{
    return std::format("Matrix4({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {})", value[0][0], value[0][1], value[0][2], value[0][3], value[1][0], value[1][1], value[1][2], value[1][3], value[2][0], value[2][1], value[2][2], value[2][3], value[3][0], value[3][1], value[3][2], value[3][3]);
}
