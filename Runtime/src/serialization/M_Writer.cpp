#include "pch.h"
#include "serialization/M_Writer.h"

#include "assets/M_Asset.h"
#include "types/M_Node.h"
#include "serialization/M_ISerializable.h"

using namespace minty;

minty::Writer::Writer(Node& node, void const* const data)
	: _node(&node)
	, _data(data)
{}

Node& minty::Writer::get_node()
{
	return *_node;
}

Node const& minty::Writer::get_node() const
{
	return *_node;
}

Node const* minty::Writer::get_node(String const& name) const
{
	return _node->find(name);
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
	return static_cast<bool>(_node->find(name));
}

void minty::Writer::write(String const& name)
{
	
}

void minty::Writer::write(Node const& node)
{
	_node->add_child(node);
}

void minty::Writer::write(String const& name, ISerializable const& value)
{
	// add child object for this object to write
	write(Node(name));

	// create a Writer to use
	Writer Writer(_node->get_children().back(), _data);

	// serialize the object into that node
	value.serialize(Writer);
}

String minty::to_string(Writer const& value)
{
	return std::format("Writer(node = {})", to_string(*value._node));
}
