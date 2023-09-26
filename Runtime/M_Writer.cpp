#include "pch.h"
#include "M_Writer.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Writer::Writer(SerializedNode& node)
	: _node(node)
{}

void minty::Writer::write(std::string const& name)
{
	write(name, "");
}

void minty::Writer::write(std::string const& name, SerializedNode const& node)
{
	_node.children.emplace(name, node);
}

void minty::Writer::write(std::string const& name, ISerializable const* const value)
{
	// add child object for this object to write
	_node.children.emplace(name, SerializedNode());

	// create a Writer to use
	Writer Writer(_node.children.at(name));

	// serialize the object into that node
	value->serialize(Writer);
}

void minty::Writer::write(std::string const& name, std::string const& value)
{
	_node.children.emplace(name, SerializedNode{ .data = value });
}

void minty::Writer::write(std::string const& name, int const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(std::string const& name, float const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(std::string const& name, byte const value)
{
	write(name, std::to_string(value));
}