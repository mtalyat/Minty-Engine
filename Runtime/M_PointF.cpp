#include "pch.h"
#include "M_PointF.h"

#include "M_Math.h"
#include "M_Point.h"
#include <format>

using namespace minty;

PointF::PointF()
    : x(0.0f), y(0.0f) 
{}

PointF::PointF(float const x, float const y)
    : x(x), y(y) 
{}

PointF::PointF(int const x, int const y)
    : x(static_cast<float>(x)), y(static_cast<float>(y)) 
{}

PointF::PointF(PointF const& other)
    : x(other.x)
    , y(other.y)
{}

Point PointF::toPoint() const
{
    return Point(roundToInt(x), roundToInt(y));
}

std::string const minty::PointF::toString() const
{
    return std::format("PointF({0}, {1})", x, y);
}

PointF minty::PointF::polarToCartesian(PointF const& polar)
{
    return PointF(cos(polar.x) * polar.y, sin(polar.x) * polar.y);
}

PointF minty::PointF::cartesianToPolar(PointF const& cartesian)
{
    return PointF(angle(cartesian.x, cartesian.y), magnitude(cartesian.x, cartesian.y));
}
