#include "pch.h"
#include "M_Vector2.h"

#include "M_Math.h"
#include "M_Vector2Int.h"
#include "M_Vector3.h"
#include <format>

using namespace minty;

Vector2::Vector2()
    : x(), y() 
{}

Vector2::Vector2(float const x, float const y)
    : x(x), y(y) 
{}

Vector2::Vector2(int const x, int const y)
    : x(static_cast<float>(x)), y(static_cast<float>(y)) 
{}

Vector2::Vector2(Vector2 const& other)
    : x(other.x)
    , y(other.y)
{}

minty::Vector2::operator Vector2Int() const
{
    return Vector2Int(x, y);
}

minty::Vector2::operator Vector3() const
{
    return Vector3(x, y);
}

std::string const minty::Vector2::toString() const
{
    return std::format("Vector2({0}, {1})", x, y);
}

minty::Vector2 minty::Vector2::polarToCartesian(minty::Vector2 const& polar)
{
    return minty::Vector2(math::cos(polar.x) * polar.y, math::sin(polar.x) * polar.y);
}

minty::Vector2 minty::Vector2::cartesianToPolar(minty::Vector2 const& cartesian)
{
    return minty::Vector2(math::angle(cartesian.x, cartesian.y), math::magnitude(cartesian.x, cartesian.y));
}
