#include "pch.h"
#include "M_Vector3Int.h"

#include "M_Math.h"
#include "M_Vector3.h"
#include "M_Vector2Int.h"
#include <format>

minty::Vector3Int::Vector3Int()
	: x(), y(), z()
{}

minty::Vector3Int::Vector3Int(int const x, int const y, int const z)
	: x(x), y(y), z(z)
{}

minty::Vector3Int::Vector3Int(float const x, float const y, float const z)
	: x(math::floor_to_int(x)), y(math::floor_to_int(y)), z(math::floor_to_int(z))
{}

minty::Vector3Int::Vector3Int(minty::Vector3Int const& other)
	: x(other.x), y(other.y), z(other.z)
{}

minty::Vector3Int::operator minty::Vector3() const
{
	return Vector3(x, y, z);
}

minty::Vector3Int::operator minty::Vector2Int() const
{
	return Vector2Int(x, y);
}

std::string const minty::Vector3Int::to_string() const
{
	return std::format("Vector3Int({}, {}, {})", x, y, z);
}
