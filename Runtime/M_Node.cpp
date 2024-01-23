#include "pch.h"
#include "M_Node.h"

#include "M_Console.h"
#include "M_File.h"
#include "M_Parse.h"
#include <vector>

using namespace minty;

String const& minty::Node::to_string() const
{
	return data;
}

Byte minty::Node::to_byte(Byte const defaultValue) const
{
	Byte out;
	if (parse::try_byte(data, out))
	{
		return out;
	}

	return defaultValue;
}

int minty::Node::to_int(int const defaultValue) const
{
	int out;
	if (parse::try_int(data, out))
	{
		return out;
	}

	return defaultValue;
}

ID minty::Node::to_id(ID const defaultValue) const
{
	ID out;
	if (parse::try_id(data, out))
	{
		return out;
	}

	return defaultValue;
}

unsigned int minty::Node::to_uint(unsigned int const defaultValue) const
{
	unsigned int out;
	if (parse::try_uint(data, out))
	{
		return out;
	}

	return defaultValue;
}

size_t minty::Node::to_size(size_t const defaultValue) const
{
	size_t out;
	if (parse::try_size(data, out))
	{
		return out;
	}

	return defaultValue;
}

float minty::Node::to_float(float const defaultValue) const
{
	float out;
	if (parse::try_float(data, out))
	{
		return out;
	}

	return defaultValue;
}

bool minty::Node::to_bool(bool const defaultValue) const
{
	bool out;
	if (parse::try_bool(data, out))
	{
		return out;
	}

	return defaultValue;
}

Node* minty::Node::find(String const& name)
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second.front();
	}

	return nullptr;
}

Node const* minty::Node::find(String const& name) const
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second.front();
	}

	return nullptr;
}

std::vector<Node>* minty::Node::find_all(String const& name)
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second;
	}

	return nullptr;
}

std::vector<Node> const* minty::Node::find_all(String const& name) const
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second;
	}

	return nullptr;
}

bool minty::Node::has_data() const
{
	return static_cast<bool>(data.size());
}

void minty::Node::print(int const indent) const
{
	// do nothing if no children
	if (!children.size()) return;

	// create indent string before the printed line
	String indentString(indent, '\t');

	// print children
	// parent takes care of printing this node's data
	for (auto const& pair : children)
	{
		for (auto const& child : pair.second)
		{
			if (child.data.size())
			{
				// print data if there is something
				console::print(std::format("{}{}: {}", indentString, pair.first, child.data));
			}
			else
			{
				// print normal if no data
				console::print(std::format("{}{}", indentString, pair.first));
			}

			// if child has children, print those, recusrively
			child.print(indent + 1);
		}
	}
}

String minty::to_string(Node const& value)
{
	return std::format("Node(data = {}, children size = {})", value.data, value.children.size());
}
