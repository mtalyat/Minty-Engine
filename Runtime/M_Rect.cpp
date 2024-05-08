#include "pch.h"
#include "M_Rect.h"

#include "M_Math.h"
#include "M_RectF.h"
#include "M_Writer.h"
#include "M_Reader.h"
#include <format>

using namespace minty;

minty::Rect::Rect()
	: x(0)
	, y(0)
	, width(0)
	, height(0)
{}

minty::Rect::Rect(int const width, int const height)
	: x(0)
	, y(0)
	, width(width)
	, height(height)
{}

minty::Rect::Rect(Vector2Int const size)
	: x(0)
	, y(0)
	, width(size.x)
	, height(size.y)
{}

minty::Rect::Rect(int const x, int const y, int const width, int const height)
	: x(x)
	, y(y)
	, width(width)
	, height(height)
{}

minty::Rect::Rect(Vector2Int const position, Vector2Int const size)
	: x(position.x)
	, y(position.y)
	, width(size.x)
	, height(size.y)
{}

bool Rect::overlaps(Rect const& other) const
{
	return left() < other.right() && right() > other.left() &&
		top() < other.bottom() && bottom() > other.top();
}

Rect Rect::overlap(Rect const& other) const
{
	int xMin = Math::clamp(other.x - x, 0, width);
	int yMin = Math::clamp(other.y - y, 0, height);

	int xMax = Math::clamp(other.x + other.width - x, 0, width);
	int yMax = Math::clamp(other.y + other.height - y, 0, height);

	return Rect(xMin, yMin, xMax - xMin, yMax - yMin);
}

bool Rect::contains(Vector2Int const& other) const
{
	return other.x >= x && other.x < x + width && other.y >= y && other.y < y + height;
}

RectF Rect::to_RectF() const
{
	return RectF(x, y, width, height);
}

void minty::Rect::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("width", width);
	writer.write("height", height);
}

void minty::Rect::deserialize(Reader const& reader)
{
	reader.try_read_int("x", x);
	reader.try_read_int("y", y);
	reader.try_read_int("width", width);
	reader.try_read_int("height", height);
}

Rect Rect::bounds(int const left, int const right, int const top, int const bottom)
{
	return Rect(left, top, right - left + 1, bottom - top + 1);
}

Rect Rect::round(RectF const& rect)
{
	return Rect(Math::round_to_int(rect.x), Math::round_to_int(rect.y), Math::round_to_int(rect.width), Math::round_to_int(rect.height));
}

String minty::to_string(Rect const& rect)
{
	return std::format("Rect({}, {}, {}, {})", rect.x, rect.y, rect.width, rect.height);
}
