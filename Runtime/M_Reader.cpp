#include "pch.h"
#include "M_Reader.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Reader::Reader(Node const& node)
	: _node(node)
{}

Node const* minty::Reader::get_node() const
{
	return &_node;
}

Node const* minty::Reader::get_node(std::string const& name) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		// found
		return &found->second;
	}

	// not found
	return nullptr;
}

void minty::Reader::read_object(std::string const& name, ISerializable* const value) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		// create Reader to use
		Reader reader(found->second);

		// deserialize the values into the given object
		value->deserialize(reader);
	}
	else
	{
		// deserialize with an empty node so it is initialized to defaults
		Node empty;
		Reader reader(empty);
		value->deserialize(reader);
	}
}

std::string minty::Reader::read_string(std::string const& name, std::string const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return found->second.data;
	}

	return defaultValue;
}

int minty::Reader::read_int(std::string const& name, int const defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return std::stoi(found->second.data);
	}

	return defaultValue;
}

float minty::Reader::read_float(std::string const& name, float const defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return std::stof(found->second.data);
	}

	return defaultValue;
}

byte minty::Reader::read_byte(std::string const& name, byte const defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return static_cast<byte>(std::stoi(found->second.data));
	}

	return defaultValue;
}

Vector2 minty::Reader::read_vector2(std::string const& name, Vector2 const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector2(r.read_float("x", defaultValue.x), r.read_float("y", defaultValue.y));
	}

	return defaultValue;
}

Vector3 minty::Reader::read_vector3(std::string const& name, Vector3 const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector3(r.read_float("x", defaultValue.x), r.read_float("y", defaultValue.y), r.read_float("z", defaultValue.z));
	}

	return defaultValue;
}

Vector4 minty::Reader::read_vector4(std::string const& name, Vector4 const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector4(r.read_float("x", defaultValue.x), r.read_float("y", defaultValue.y), r.read_float("z", defaultValue.z), r.read_float("w", defaultValue.w));
	}

	return defaultValue;
}

Vector2Int minty::Reader::read_vector2int(std::string const& name, Vector2Int const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector2Int(r.read_int("x", defaultValue.x), r.read_int("y", defaultValue.y));
	}

	return defaultValue;
}

Vector3Int minty::Reader::read_vector3int(std::string const& name, Vector3Int const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector3Int(r.read_int("x", defaultValue.x), r.read_int("y", defaultValue.y), r.read_int("z", defaultValue.z));
	}

	return defaultValue;
}

Vector4Int minty::Reader::read_vector4int(std::string const& name, Vector4Int const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		return Vector4Int(r.read_int("x", defaultValue.x), r.read_int("y", defaultValue.y), r.read_int("z", defaultValue.z), r.read_int("w", defaultValue.w));
	}

	return defaultValue;
}

Quaternion minty::Reader::read_quaternion(std::string const& name, Quaternion const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		Reader r(found->second);

		Vector3 defaultValueEuler = defaultValue.to_euler_angles();

		return Quaternion::from_euler_angles(r.read_float("x", defaultValueEuler.x), r.read_float("y", defaultValueEuler.y), r.read_float("z", defaultValueEuler.z));
	}

	return defaultValue;
}

std::string minty::to_string(Reader const& value)
{
	return std::format("Reader(node = {})", to_string(value._node));
}
