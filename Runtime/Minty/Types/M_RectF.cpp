#include "pch.h"
#include "Minty/Types/M_RectF.h"

#include "Minty/Math/M_Math.h"
#include "Minty/Types/M_Rect.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"
#include <format>

using namespace Minty;

bool RectF::overlaps(RectF const& other) const
{
	return left() < other.right() && right() > other.left() &&
		top() < other.bottom() && bottom() > other.top();
}

RectF RectF::overlap(RectF const& other) const
{
	float xMin = Math::clamp(other.x - x, 0.0f, width);
	float yMin = Math::clamp(other.y - y, 0.0f, height);

	float xMax = Math::clamp(other.x + other.width - x, 0.0f, width);
	float yMax = Math::clamp(other.y + other.height - y, 0.0f, height);

	return RectF(xMin, yMin, xMax - xMin, yMax - yMin);
}

bool RectF::contains(Vector2 const& other) const
{
	return other.x >= x && other.x < x + width && other.y >= y && other.y < y + height;
}

void Minty::RectF::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("width", width);
	writer.write("height", height);
}

void Minty::RectF::deserialize(Reader const& reader)
{
	reader.try_read_float("x", x);
	reader.try_read_float("y", y);
	reader.try_read_float("width", width);
	reader.try_read_float("height", height);
}

RectF RectF::bounds(float const left, float const right, float const top, float const bottom)
{
	return RectF(left, top, right - left, bottom - top);
}

Rect RectF::floor(RectF const& rect)
{
	return Rect(Math::floor_to_int(rect.x), Math::floor_to_int(rect.y), Math::floor_to_int(rect.width), Math::floor_to_int(rect.height));
}

Rect RectF::round(RectF const& rect)
{
	return Rect(Math::round_to_int(rect.x), Math::round_to_int(rect.y), Math::round_to_int(rect.width), Math::round_to_int(rect.height));
}

Rect RectF::ceil(RectF const& rect)
{
	return Rect(Math::ceil_to_int(rect.x), Math::ceil_to_int(rect.y), Math::ceil_to_int(rect.width), Math::ceil_to_int(rect.height));
}

String Minty::to_string(RectF const& value)
{
	return std::format("RectF({0}, {1}, {2}, {3})", value.x, value.y, value.width, value.height);
}
