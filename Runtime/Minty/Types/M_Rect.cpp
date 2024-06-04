#include "pch.h"
#include "Minty/Types/M_Rect.h"

#include "Minty/Math/M_Math.h"
#include "Minty/Types/M_RectF.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"
#include <format>

using namespace Minty;

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

void Minty::Rect::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("width", width);
	writer.write("height", height);
}

void Minty::Rect::deserialize(Reader const& reader)
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

String Minty::to_string(Rect const& rect)
{
	return std::format("Rect({}, {}, {}, {})", rect.x, rect.y, rect.width, rect.height);
}
