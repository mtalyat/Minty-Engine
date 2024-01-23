#include "pch.h"
#include "M_Vector.h"

#include <format>

using namespace minty;

std::istream& minty::operator>>(std::istream& stream, Vector2& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector2& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ')';
	return stream;
}

String minty::to_string(Vector2 const& value)
{
	return std::format("({}, {})", value.x, value.y);
}

Vector2 minty::from_string_vector2(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector2();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector2(parse::to_float(parts.at(0)), parse::to_float(parts.at(1)));
}

std::istream& minty::operator>>(std::istream& stream, Vector3& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard >> vector.z >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector3& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ", " << vector.z << ')';
	return stream;
}

String minty::to_string(Vector3 const& v)
{
	return std::format("({}, {}, {})", v.x, v.y, v.z);
}

Vector3 minty::from_string_vector3(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector3();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector3(parse::to_float(parts.at(0)), parse::to_float(parts.at(1)), parse::to_float(parts.at(2)));
}

std::istream& minty::operator>>(std::istream& stream, Vector4& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard >> vector.z >> discard >> vector.w >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector4& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ')';
	return stream;
}

String minty::to_string(Vector4 const& v)
{
	return std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w);
}

Vector4 minty::from_string_vector4(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector4();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector4(parse::to_float(parts.at(0)), parse::to_float(parts.at(1)), parse::to_float(parts.at(2)), parse::to_float(parts.at(3)));
}

std::istream& minty::operator>>(std::istream& stream, Vector2Int& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector2Int& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ')';
	return stream;
}

String minty::to_string(Vector2Int const& v)
{
	return std::format("({}, {})", v.x, v.y);
}

Vector2Int minty::from_string_vector2int(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector2Int();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector2Int(parse::to_int(parts.at(0)), parse::to_int(parts.at(1)));
}

std::istream& minty::operator>>(std::istream& stream, Vector3Int& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard >> vector.z >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector3Int& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ", " << vector.z << ')';
	return stream;
}

String minty::to_string(Vector3Int const& v)
{
	return std::format("({}, {}, {})", v.x, v.y, v.z);
}

Vector3Int minty::from_string_vector3int(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector3Int();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector3Int(parse::to_int(parts.at(0)), parse::to_int(parts.at(1)), parse::to_int(parts.at(2)));
}

std::istream& minty::operator>>(std::istream& stream, Vector4Int& vector)
{
	char discard;
	stream >> discard >> vector.x >> discard >> vector.y >> discard >> vector.z >> discard >> vector.w >> discard;
	return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Vector4Int& vector)
{
	stream << '(' << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ')';
	return stream;
}

String minty::to_string(Vector4Int const& v)
{
	return std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w);
}

Vector4Int minty::from_string_vector4int(String const& value)
{
	// cannot parse if does not start with ( or )
	if (!value.starts_with("(") || !value.ends_with(")"))
	{
		return Vector4Int();
	}

	// split by ", ", grab inner values
	String raw = value.substr(1, value.size() - 2);
	std::vector<String> parts = string::split(raw, ", ");
	return Vector4Int(parse::to_int(parts.at(0)), parse::to_int(parts.at(1)), parse::to_int(parts.at(2)), parse::to_int(parts.at(3)));
}

minty::Vector2::Vector2(glm::vec2 const& vec)
	: glm::vec2::vec(vec)
{}

void minty::Vector2::serialize(Writer& writer) const
{
	writer.write("x", x, 0.0f);
	writer.write("y", y, 0.0f);
}

void minty::Vector2::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
}

Vector2 minty::Vector2::zero()
{
	return Vector2(0.0f, 0.0f);
}

Vector2 minty::Vector2::half()
{
	return Vector2(0.5f, 0.5f);
}

Vector2 minty::Vector2::one()
{
	return Vector2(1.0f, 1.0f);
}

Vector2 minty::Vector2::right()
{
	return Vector2(1.0f, 0.0f);
}

Vector2 minty::Vector2::up()
{
	return Vector2(0.0f, -1.0f);
}

Vector2 minty::Vector2::left()
{
	return Vector2(-1.0f, 0.0f);
}

Vector2 minty::Vector2::down()
{
	return Vector2(0.0f, 1.0f);
}

minty::Vector3::Vector3(glm::vec3 const& vec)
	: glm::vec3::vec(vec)
{}

void minty::Vector3::serialize(Writer& writer) const
{
	writer.write("x", x, 0.0f);
	writer.write("y", y, 0.0f);
	writer.write("z", z, 0.0f);
}

void minty::Vector3::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
}

minty::Vector4::Vector4(glm::vec4 const& vec)
	: glm::vec4::vec(vec)
{}

void minty::Vector4::serialize(Writer& writer) const
{
	writer.write("x", x, 0.0f);
	writer.write("y", y, 0.0f);
	writer.write("z", z, 0.0f);
	writer.write("w", w, 0.0f);
}

void minty::Vector4::deserialize(Reader const& reader)
{
	x = reader.read_float("x");
	y = reader.read_float("y");
	z = reader.read_float("z");
	w = reader.read_float("w");
}

minty::Vector2Int::Vector2Int(glm::ivec2 const& vec)
	: glm::ivec2::vec(vec)
{}

void minty::Vector2Int::serialize(Writer& writer) const
{
	writer.write("x", x, 0);
	writer.write("y", y, 0);
}

void minty::Vector2Int::deserialize(Reader const& reader)
{
	x = reader.read_int("x");
	y = reader.read_int("y");
}

minty::Vector3Int::Vector3Int(glm::ivec3 const& vec)
	: glm::ivec3::vec(vec)
{}

void minty::Vector3Int::serialize(Writer& writer) const
{
	writer.write("x", x, 0);
	writer.write("y", y, 0);
	writer.write("z", z, 0);
}

void minty::Vector3Int::deserialize(Reader const& reader)
{
	x = reader.read_int("x");
	y = reader.read_int("y");
	z = reader.read_int("z");
}

minty::Vector4Int::Vector4Int(glm::ivec4 const& vec)
	: glm::ivec4::vec(vec)
{}

void minty::Vector4Int::serialize(Writer& writer) const
{
	writer.write("x", x, 0);
	writer.write("y", y, 0);
	writer.write("z", z, 0);
	writer.write("w", w, 0);
}

void minty::Vector4Int::deserialize(Reader const& reader)
{
	x = reader.read_int("x");
	y = reader.read_int("y");
	z = reader.read_int("z");
	w = reader.read_int("w");
}

Vector3 minty::Vector3::zero()
{
	return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 minty::Vector3::half()
{
	return Vector3(0.5f, 0.5f, 0.5f);
}

Vector3 minty::Vector3::one()
{
	return Vector3(1.0f, 1.0f, 1.0f);
}

Vector3 minty::Vector3::right()
{
	return Vector3(1.0f, 0.0f, 0.0f);
}

Vector3 minty::Vector3::up()
{
	return Vector3(0.0f, -1.0f, 0.0f);
}

Vector3 minty::Vector3::left()
{
	return Vector3(-1.0f, 0.0f, 0.0f);
}

Vector3 minty::Vector3::down()
{
	return Vector3(0.0f, 1.0f, 0.0f);
}

Vector3 minty::Vector3::forward()
{
	return Vector3(0.0f, 0.0f, 1.0f);
}

Vector3 minty::Vector3::backward()
{
	return Vector3(0.0f, 0.0f, -1.0f);
}
