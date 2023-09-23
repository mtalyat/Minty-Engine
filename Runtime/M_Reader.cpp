#include "pch.h"
#include "M_Reader.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Reader::Reader(SerializedNode const& node)
	: _node(node)
{}

void minty::Reader::read_object(std::string const& name, ISerializable* const value) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		// create Reader to use
		Reader Reader(found->second);

		// deserialize the values into the given object
		value->deserialize(Reader);
	}

	// do nothing if not found
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