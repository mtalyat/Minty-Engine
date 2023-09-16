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
    : x(math::roundToInt(x))
    , y(math::roundToInt(y))
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

Vector2Int minty::Vector2Int::polarToCartesian(Vector2Int const& polar)
{
    return Vector2Int(math::cos(polar.x) * polar.y, math::sin(polar.x) * polar.y);
}

Vector2Int minty::Vector2Int::cartesianToPolar(Vector2Int const& cartesian)
{
    return Vector2Int(math::angle(cartesian.x, cartesian.y), math::magnitude(cartesian.x, cartesian.y));
}
