#include "pch.h"
#include "M_Vector3.h"

#include "M_Vector3Int.h"
#include "M_Vector2.h"
#include <format>

minty::Vector3::Vector3()
	: x(), y(), z()
{}

minty::Vector3::Vector3(float const x, float const y, float const z)
	: x(x), y(y), z(z)
{}

minty::Vector3::Vector3(int const x, int const y, int const z)
	: x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z))
{}

minty::Vector3::Vector3(Vector3 const& other)
	: x(other.x), y(other.y), z(other.z)
{}

minty::Vector3::operator minty::Vector3Int() const
{
	return Vector3Int(x, y, z);
}

minty::Vector3::operator minty::Vector2() const
{
	return Vector2(x, y);
}

std::string const minty::Vector3::to_string() const
{
	return std::format("Vector3({}, {}, {})", x, y, z);
}

void minty::Vector3::serialize(Writer& writer) const
{
	writer.write("x", x);
	writer.write("y", y);
	writer.write("z", z);
}

void minty::Vector3::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
}
