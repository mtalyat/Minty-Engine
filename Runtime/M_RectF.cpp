#include "pch.h"
#include "M_RectF.h"

#include "M_Math.h"
#include "M_Rect.h"
#include <format>

using namespace minty;

minty::RectF::RectF()
	: x(0)
	, y(0)
	, width(0)
	, height(0)
{}

minty::RectF::RectF(float const width, float const height)
	: x(0)
	, y(0)
	, width(width)
	, height(height)
{}

minty::RectF::RectF(Vector2 const size)
	: x(0)
	, y(0)
	, width(size.x)
	, height(size.y)
{}

minty::RectF::RectF(float const x, float const y, float const width, float const height)
	: x(x)
	, y(y)
	, width(width)
	, height(height)
{}

minty::RectF::RectF(int const x, int const y, int const width, int const height)
	: x(static_cast<float>(x))
	, y(static_cast<float>(y))
	, width(static_cast<float>(width))
	, height(static_cast<float>(height))
{}

minty::RectF::RectF(Vector2 const position, Vector2 const size)
	: x(position.x)
	, y(position.y)
	, width(size.x)
	, height(size.y)
{}

minty::RectF::RectF(Vector2Int const position, Vector2Int const size)
	: x(static_cast<float>(position.x))
	, y(static_cast<float>(position.y))
	, width(static_cast<float>(size.x))
	, height(static_cast<float>(size.y))
{}

std::string const minty::RectF::toString() const
{
	return std::format("RectF({0}, {1}, {2}, {3})", x, y, width, height);
}


bool RectF::overlaps(RectF const& other) const
{
	return left() < other.right() && right() > other.left() &&
		top() < other.bottom() && bottom() > other.top();
}

RectF RectF::overlap(RectF const& other) const
{
	float xMin = math::clamp(other.x - x, 0.0f, width);
	float yMin = math::clamp(other.y - y, 0.0f, height);

	float xMax = math::clamp(other.x + other.width - x, 0.0f, width);
	float yMax = math::clamp(other.y + other.height - y, 0.0f, height);

	return RectF(xMin, yMin, xMax - xMin, yMax - yMin);
}

bool RectF::contains(Vector2 const& other) const
{
	return other.x >= x && other.x < x + width && other.y >= y && other.y < y + height;
}

RectF RectF::bounds(float const left, float const right, float const top, float const bottom)
{
	return RectF(left, top, right - left, bottom - top);
}

Rect RectF::floor(RectF const& rect)
{
	return Rect(math::floorToInt(rect.x), math::floorToInt(rect.y), math::floorToInt(rect.width), math::floorToInt(rect.height));
}

Rect RectF::round(RectF const& rect)
{
	return Rect(math::roundToInt(rect.x), math::roundToInt(rect.y), math::roundToInt(rect.width), math::roundToInt(rect.height));
}

Rect RectF::ceil(RectF const& rect)
{
	return Rect(math::ceilToInt(rect.x), math::ceilToInt(rect.y), math::ceilToInt(rect.width), math::ceilToInt(rect.height));
}