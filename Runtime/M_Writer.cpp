#include "pch.h"
#include "M_Writer.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Writer::Writer(Node& node, void const* const data)
	: _node(node)
	, _data(data)
{}

Node& minty::Writer::get_node()
{
	return _node;
}

Node const& minty::Writer::get_node() const
{
	return _node;
}

Node const* minty::Writer::get_node(String const& name) const
{
	return _node.find(name);
}

void const* minty::Writer::get_data() const
{
	return _data;
}

void minty::Writer::set_data(void const* const data)
{
	_data = data;
}

bool minty::Writer::exists(String const& name) const
{
	return static_cast<bool>(_node.find(name));
}

void minty::Writer::write(String const& name)
{
	write(name, "");
}

void minty::Writer::write(String const& name, Node const& node)
{
	auto const& found = _node.children.find(name);

	if (found != _node.children.end())
	{
		// existing key/children list
		found->second.push_back(node);
	}
	else
	{
		// new key and children list
		_node.children.emplace(name, std::vector{ node });
	}
}

void minty::Writer::write(String const& name, ISerializable const* const value)
{
	// add child object for this object to write
	write(name, Node());

	// create a Writer to use
	Writer Writer(_node.children.at(name).back(), _data);

	// serialize the object into that node
	value->serialize(Writer);
}

void minty::Writer::write(String const& name, String const& value)
{
	write(name, Node{.data = value });
}

void minty::Writer::write(String const& name, int const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, unsigned int const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, float const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, Byte const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, size_t const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, bool const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(String const& name, String const& value, String const& defaultValue)
{
	if (value.compare(defaultValue))
	{
		// not default value
		write(name, value);
	}
}

void minty::Writer::write(String const& name, int const value, int const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

void minty::Writer::write(String const& name, unsigned int const value, unsigned int const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

void minty::Writer::write(String const& name, float const value, float const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

void minty::Writer::write(String const& name, Byte const value, Byte const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

void minty::Writer::write(String const& name, size_t const value, size_t const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

void minty::Writer::write(String const& name, bool const value, bool const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, value);
	}
}

String minty::to_string(Writer const& value)
{
	return std::format("Writer(node = {})", to_string(value._node));
}
