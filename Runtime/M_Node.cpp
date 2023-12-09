#include "pch.h"
#include "M_Node.h"

#include "M_Console.h"
#include "M_File.h"
#include "M_Parse.h"
#include <vector>

using namespace minty;

std::string const& minty::Node::to_string() const
{
	return data;
}

byte minty::Node::to_byte(byte const defaultValue) const
{
	byte out;
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

size_t minty::Node::to_size_t(size_t const defaultValue) const
{
	size_t out;
	if (parse::try_size_t(data, out))
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

Node* minty::Node::find(std::string const& name)
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second.front();
	}

	return nullptr;
}

Node const* minty::Node::find(std::string const& name) const
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second.front();
	}

	return nullptr;
}

std::vector<Node>* minty::Node::find_all(std::string const& name)
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second;
	}

	return nullptr;
}

std::vector<Node> const* minty::Node::find_all(std::string const& name) const
{
	auto const& found = children.find(name);

	if (found != children.end())
	{
		return &found->second;
	}

	return nullptr;
}

std::string const& minty::Node::get_string(std::string const& name, std::string const& defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_string();
	}
	else
	{
		return defaultValue;
	}
}

byte minty::Node::get_byte(std::string const& name, byte const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_byte(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

int minty::Node::get_int(std::string const& name, int const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_int(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

ID minty::Node::get_id(std::string const& name, ID const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_id(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

unsigned int minty::Node::get_uint(std::string const& name, unsigned int const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_uint(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

size_t minty::Node::get_size_t(std::string const& name, size_t const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_size_t(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

float minty::Node::get_float(std::string const& name, float const defaultValue) const
{
	Node const* node = find(name);
	if (node)
	{
		return node->to_float(defaultValue);
	}
	else
	{
		return defaultValue;
	}
}

void minty::Node::print(int const indent) const
{
	// print children
	// parent takes care of printing this node's data
	for (auto const& pair : children)
	{
		for (auto const& child : pair.second)
		{
			if (child.data.size())
			{
				// print data if there is something
				console::print(std::format("{}{}: {}", std::string(indent, '\t'), pair.first, child.data));
			}
			else
			{
				// print normal if no data
				console::print(std::format("{}{}", std::string(indent, '\t'), pair.first));
			}

			// if child has children, print those, recusrively
			child.print(indent + 1);
		}
	}
}

std::string minty::to_string(Node const& value)
{
	return std::format("Node(data = {}, children size = {})", value.data, value.children.size());
}
