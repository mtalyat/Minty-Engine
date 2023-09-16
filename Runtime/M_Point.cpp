#include "pch.h"
#include "M_Point.h"

#include "M_Math.h"
#include "M_PointF.h"
#include <format>

using namespace minty;

minty::Point::Point()
    : x(0), y(0) 
{}

minty::Point::Point(int const _x, int const _y)
    : x(_x), y(_y) 
{}

minty::Point::Point(float const _x, float const _y)
    : x(roundToInt(_x))
    , y(roundToInt(_y))
{}

minty::Point::Point(Point const& other)
    : x(other.x)
    , y(other.y)
{}

PointF minty::Point::toPointF() const
{
    return PointF(static_cast<float>(x), static_cast<float>(y));
}

inline std::string const minty::Point::toString() const
{
    return std::format("Point({0}, {1})", x, y);
}

Point minty::Point::polarToCartesian(Point const& polar)
{
    return Point(cos(polar.x) * polar.y, sin(polar.x) * polar.y);
}

Point minty::Point::cartesianToPolar(Point const& cartesian)
{
    return Point(angle(cartesian.x, cartesian.y), magnitude(cartesian.x, cartesian.y));
}
