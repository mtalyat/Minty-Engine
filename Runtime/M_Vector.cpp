#include "pch.h"
#include "M_Vector.h"

#include <format>

using namespace minty;

String minty::to_string(Vector2 const& v)
{
	return std::format("({}, {})", v.x, v.y);
}

String minty::to_string(Vector3 const& v)
{
	return std::format("({}, {}, {})", v.x, v.y, v.z);
}

String minty::to_string(Vector4 const& v)
{
	return std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w);
}

String minty::to_string(Vector2Int const& v)
{
	return std::format("({}, {})", v.x, v.y);
}

String minty::to_string(Vector3Int const& v)
{
	return std::format("({}, {}, {})", v.x, v.y, v.z);
}

String minty::to_string(Vector4Int const& v)
{
	return std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w);
}
