#include "pch.h"
#include "M_Reader.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Reader::Reader(Node const& node, void* const data)
	: _node(node)
	, _data(data)
{}

Node const& minty::Reader::get_node() const
{
	return _node;
}

Node const* minty::Reader::get_node(String const& name) const
{
	return _node.find(name);
}

void* minty::Reader::get_data() const
{
	return _data;
}

bool minty::Reader::exists(String const& name) const
{
	return static_cast<bool>(_node.find(name));
}

void minty::Reader::read_object(String const& name, ISerializable* const value) const
{
	auto const* found = _node.find(name);
	if (found)
	{
		// create Reader to use
		Reader reader(*found, _data);

		// deserialize the values into the given object
		value->deserialize(reader);
	}
	else
	{
		// deserialize with an empty node so it is initialized to defaults
		Node empty;
		Reader reader(empty, _data);
		value->deserialize(reader);
	}
}

String minty::Reader::read_string(String const& name, String const& defaultValue) const
{
	return _node.get_string(name, defaultValue);
}

int minty::Reader::read_int(String const& name, int const defaultValue) const
{
	return _node.get_int(name, defaultValue);
}

unsigned int minty::Reader::read_uint(String const& name, unsigned int const defaultValue) const
{
	return _node.get_uint(name, defaultValue);
}

ID minty::Reader::read_id(String const& name, ID const defaultValue) const
{
	return _node.get_id(name, defaultValue);
}

float minty::Reader::read_float(String const& name, float const defaultValue) const
{
	return _node.get_float(name, defaultValue);
}

Byte minty::Reader::read_byte(String const& name, Byte const defaultValue) const
{
	return _node.get_byte(name, defaultValue);
}

size_t minty::Reader::read_size(String const& name, size_t const defaultValue) const
{
	return _node.get_size(name, defaultValue);
}

Vector2 minty::Reader::read_vector2(String const& name, Vector2 const& defaultValue) const
{
	return _node.get_vector2(name, defaultValue);
}

Vector3 minty::Reader::read_vector3(String const& name, Vector3 const& defaultValue) const
{
	return _node.get_vector3(name, defaultValue);
}

Vector4 minty::Reader::read_vector4(String const& name, Vector4 const& defaultValue) const
{
	return _node.get_vector4(name, defaultValue);
}

Vector2Int minty::Reader::read_vector2int(String const& name, Vector2Int const& defaultValue) const
{
	return _node.get_vector2int(name, defaultValue);
}

Vector3Int minty::Reader::read_vector3int(String const& name, Vector3Int const& defaultValue) const
{
	return _node.get_vector3int(name, defaultValue);
}

Vector4Int minty::Reader::read_vector4int(String const& name, Vector4Int const& defaultValue) const
{
	return _node.get_vector4int(name, defaultValue);
}

Quaternion minty::Reader::read_quaternion(String const& name, Quaternion const& defaultValue) const
{
	return _node.get_quaternion(name, defaultValue);
}

std::vector<String> minty::Reader::read_vector_string(String const& name, std::vector<String> const& defaultValue) const
{
	return _node.get_vector_string(name, defaultValue);
}

String minty::to_string(Reader const& value)
{
	return std::format("Reader(node = {})", to_string(value._node));
}
