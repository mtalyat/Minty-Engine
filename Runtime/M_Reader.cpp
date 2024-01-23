#include "pch.h"
#include "M_Reader.h"

#include "M_Node.h"
#include "M_ISerializable.h"

using namespace minty;

minty::Reader::Reader(Node const& node, void const* const data)
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

void const* minty::Reader::get_data() const
{
	return _data;
}

void minty::Reader::set_data(void const* data)
{
	_data = data;
}

bool minty::Reader::exists(String const& name) const
{
	return static_cast<bool>(_node.find(name));
}

void minty::Reader::to_serializable(ISerializable* const value) const
{
	value->deserialize(*this);
}

void minty::Reader::read_serializable(String const& name, ISerializable* const value) const
{
	if (Node const* found = _node.find(name))
	{
		Reader reader(*found, _data);
		value->deserialize(reader);
	}
}

String minty::Reader::to_string(String const& defaultValue) const
{
	return _node.to_string();
}

String minty::Reader::read_string(String const& name, String const& defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_string();
	}

	return defaultValue;
}

int minty::Reader::to_int(int const defaultValue) const
{
	return _node.to_int(defaultValue);
}

int minty::Reader::read_int(String const& name, int const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_int(defaultValue);
	}

	return defaultValue;
}

unsigned int minty::Reader::to_uint(unsigned int const defaultValue) const
{
	return _node.to_uint(defaultValue);
}

unsigned int minty::Reader::read_uint(String const& name, unsigned int const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_uint(defaultValue);
	}

	return defaultValue;
}

ID minty::Reader::to_id(ID const defaultValue) const
{
	return _node.to_id(defaultValue);
}

ID minty::Reader::read_id(String const& name, ID const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_id(defaultValue);
	}

	return defaultValue;
}

float minty::Reader::to_float(float const defaultValue) const
{
	return _node.to_float(defaultValue);
}

float minty::Reader::read_float(String const& name, float const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_float(defaultValue);
	}

	return defaultValue;
}

Byte minty::Reader::to_byte(Byte const defaultValue) const
{
	return _node.to_byte(defaultValue);
}

Byte minty::Reader::read_byte(String const& name, Byte const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_byte(defaultValue);
	}

	return defaultValue;
}

size_t minty::Reader::to_size(size_t const defaultValue) const
{
	return _node.to_size(defaultValue);
}

size_t minty::Reader::read_size(String const& name, size_t const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_size(defaultValue);
	}

	return defaultValue;
}

bool minty::Reader::to_bool(bool const defaultValue) const
{
	return _node.to_bool(defaultValue);
}

bool minty::Reader::read_bool(String const& name, bool const defaultValue) const
{
	if (Node const* child = _node.find(name))
	{
		return child->to_bool(defaultValue);
	}

	return defaultValue;
}

String minty::to_string(Reader const& value)
{
	return std::format("Reader(node = {})", to_string(value._node));
}
