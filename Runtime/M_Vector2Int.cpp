#include "pch.h"
#include "M_Vector2Int.h"

#include "M_Math.h"
#include "M_Vector2.h"
#include "M_Vector3Int.h"
#include <format>

using namespace minty;

minty::Vector2Int::Vector2Int()
    : x(), y() 
{}

minty::Vector2Int::Vector2Int(int const x, int const y)
    : x(x), y(y) 
{}

minty::Vector2Int::Vector2Int(float const x, float const y)
    : x(math::floorToInt(x))
    , y(math::floorToInt(y))
{}

minty::Vector2Int::Vector2Int(Vector2Int const& other)
    : x(other.x)
    , y(other.y)
{}

minty::Vector2Int::operator Vector2() const
{
    return Vector2(x, y);
}

minty::Vector2Int::operator Vector3Int() const
{
    return Vector3Int(x, y);
}

inline std::string const minty::Vector2Int::toString() const
{
    return std::format("Vector2Int({0}, {1})", x, y);
}