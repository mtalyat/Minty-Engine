#include "pch.h"
#include "M_Matrix.h"

std::string minty::to_string(Matrix4 const& value)
{
    return std::format("Matrix4({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {})", value[0][0], value[0][1], value[0][2], value[0][3], value[1][0], value[1][1], value[1][2], value[1][3], value[2][0], value[2][1], value[2][2], value[2][3], value[3][0], value[3][1], value[3][2], value[3][3]);
}