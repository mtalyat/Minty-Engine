#include "pch.h"
#include "Minty/Serialization/M_Writer.h"

#include "Minty/Assets/M_Asset.h"
#include "Minty/Types/M_Node.h"
#include "Minty/Serialization/M_ISerializable.h"

using namespace Minty;

Minty::Writer::Writer(Node& node, void const* const data)
	: _node(&node)
	, _data(data)
{}

bool Minty::Writer::exists(String const& name) const
{
	return static_cast<bool>(_node->find(name));
}

void Minty::Writer::write(String const& name)
{
	write(Node(name));
}

void Minty::Writer::write(Node const& node)
{
	_node->add_child(node);
}

void Minty::Writer::write(String const& name, ISerializable const& value)
{
	// add child object for this object to write
	write(Node(name));

	// create a Writer to use
	Writer Writer(_node->get_children().back(), _data);

	// serialize the object into that node
	value.serialize(Writer);
}

String Minty::to_string(Writer const& value)
{
	return std::format("Writer(node = {})", to_string(*value._node));
}
