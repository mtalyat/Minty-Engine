#include "pch.h"
#include "types/M_Vector.h"

#include "assets/M_Text.h"
#include "tools/M_Parse.h"
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector2(Parse::to_float(parts.at(0)), Parse::to_float(parts.at(1)));
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector3(Parse::to_float(parts.at(0)), Parse::to_float(parts.at(1)), Parse::to_float(parts.at(2)));
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector4(Parse::to_float(parts.at(0)), Parse::to_float(parts.at(1)), Parse::to_float(parts.at(2)), Parse::to_float(parts.at(3)));
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector2Int(Parse::to_int(parts.at(0)), Parse::to_int(parts.at(1)));
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector3Int(Parse::to_int(parts.at(0)), Parse::to_int(parts.at(1)), Parse::to_int(parts.at(2)));
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
	std::vector<String> parts = Text::split(raw, ", ");
	return Vector4Int(Parse::to_int(parts.at(0)), Parse::to_int(parts.at(1)), Parse::to_int(parts.at(2)), Parse::to_int(parts.at(3)));
}