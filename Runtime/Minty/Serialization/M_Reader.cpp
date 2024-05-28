#include "pch.h"
#include "Minty/Serialization/M_Reader.h"

#include "Minty/Types/M_Node.h"
#include "Minty/Serialization/M_ISerializable.h"

using namespace Minty;

Minty::Reader::Reader(Node const& node, void const* const data)
	: _node(&node)
	, _data(data)
{}

bool Minty::Reader::exists(String const& name) const
{
	return static_cast<bool>(_node->find(name));
}

void Minty::Reader::to_serializable(ISerializable& value) const
{
	value.deserialize(*this);
}

void Minty::Reader::read_serializable(String const& name, ISerializable& value) const
{
	try_read_serializable(name, value);
}

bool Minty::Reader::try_read_serializable(String const& name, ISerializable& value) const
{
	if (Node const* found = _node->find(name))
	{
		Reader reader(*found, _data);
		value.deserialize(reader);
		return true;
	}
	return false;
}

String Minty::Reader::to_string(String const& defaultValue) const
{
	return _node->to_string();
}

String Minty::Reader::read_string(String const& name, String const& defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_string();
	}

	return defaultValue;
}

bool Minty::Reader::try_read_string(String const& name, String& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_string();
		return true;
	}
	return false;
}

int Minty::Reader::to_int(int const defaultValue) const
{
	return _node->to_int(defaultValue);
}

int Minty::Reader::read_int(String const& name, int const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_int(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_int(String const& name, int& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_int();
		return true;
	}
	return false;
}

unsigned int Minty::Reader::to_uint(unsigned int const defaultValue) const
{
	return _node->to_uint(defaultValue);
}

unsigned int Minty::Reader::read_uint(String const& name, unsigned int const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_uint(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_uint(String const& name, unsigned int& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_uint();
		return true;
	}
	return false;
}

ID Minty::Reader::to_id(ID const defaultValue) const
{
	return _node->to_id(defaultValue);
}

ID Minty::Reader::read_id(String const& name, ID const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_id(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_id(String const& name, ID& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_id();
		return true;
	}
	return false;
}

UUID Minty::Reader::to_uuid() const
{
	return _node->to_uuid();
}

UUID Minty::Reader::read_uuid(String const& name, UUID const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_uuid();
	}

	return defaultValue;
}

bool Minty::Reader::try_read_uuid(String const& name, UUID& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_uuid();
		return true;
	}
	return false;
}

float Minty::Reader::to_float(float const defaultValue) const
{
	return _node->to_float(defaultValue);
}

float Minty::Reader::read_float(String const& name, float const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_float(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_float(String const& name, float& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_float();
		return true;
	}
	return false;
}

Byte Minty::Reader::to_byte(Byte const defaultValue) const
{
	return _node->to_byte(defaultValue);
}

Byte Minty::Reader::read_byte(String const& name, Byte const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_byte(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_byte(String const& name, Byte& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_byte();
		return true;
	}
	return false;
}

size_t Minty::Reader::to_size(size_t const defaultValue) const
{
	return _node->to_size(defaultValue);
}

size_t Minty::Reader::read_size(String const& name, size_t const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_size(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_size(String const& name, size_t& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_size();
		return true;
	}
	return false;
}

bool Minty::Reader::to_bool(bool const defaultValue) const
{
	return _node->to_bool(defaultValue);
}

bool Minty::Reader::read_bool(String const& name, bool const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return child->to_bool(defaultValue);
	}

	return defaultValue;
}

bool Minty::Reader::try_read_bool(String const& name, bool& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = child->to_bool();
		return true;
	}
	return false;
}

Color Minty::Reader::to_color(Color const defaultValue) const
{
	return Color::fromHex(_node->to_string());
}

Color Minty::Reader::read_color(String const& name, Color const defaultValue) const
{
	if (Node const* child = _node->find(name))
	{
		return Color::fromHex(child->to_string());
	}

	return defaultValue;
}

bool Minty::Reader::try_read_color(String const& name, Color& value) const
{
	if (Node const* child = _node->find(name))
	{
		value = Color::fromHex(child->to_string());
		return true;
	}
	return false;
}

String Minty::to_string(Reader const& value)
{
	return std::format("Reader(node = {})", to_string(*value._node));
}
