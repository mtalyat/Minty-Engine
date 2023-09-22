#include "pch.h"
#include "M_Serializer.h"

#include "M_ISerializable.h"
#include "M_File.h"
#include "M_Console.h"

using namespace minty;

minty::Serializer::Serializer(SerializedNode& node)
	: _node(node)
{}

void minty::Serializer::write(std::string const& name, ISerializable const& value)
{
	// add child object for this object to write
	_node.children.emplace(name, SerializedNode());

	// create a serializer to use
	Serializer serializer(_node.children.at(name));

	// serialize the object into that node
	value.serialize(serializer);
}

void minty::Serializer::write(std::string const& name, std::string const& value)
{
	_node.children.emplace(name, SerializedNode{ .data = value });
}

void minty::Serializer::write(std::string const& name, int const value)
{
	write(name, std::to_string(value));
}

void minty::Serializer::write(std::string const& name, byte const value)
{
	write(name, std::to_string(value));
}

void minty::Serializer::read_object(std::string const& name, ISerializable& value) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		// create serializer to use
		Serializer serializer(found->second);

		// deserialize the values into the given object
		value.deserialize(serializer);
	}

	// do nothing if not found
}

std::string minty::Serializer::read_string(std::string const& name, std::string const& defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return found->second.data;
	}

	return defaultValue;
}

int minty::Serializer::read_int(std::string const& name, int const defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return std::stoi(found->second.data);
	}

	return defaultValue;
}

byte minty::Serializer::read_byte(std::string const& name, byte const defaultValue) const
{
	auto const& found = _node.children.find(name);
	if (found != _node.children.end())
	{
		return static_cast<byte>(std::stoi(found->second.data));
	}

	return defaultValue;
}

SerializedNode minty::Serializer::parse_file(std::string const& path)
{
	std::vector<std::string> lines = file::read_all_lines(path);

	int indent = 0;
	std::string key;
	std::string value;

	SerializedNode root;
	
	std::vector<SerializedNode*> nodeStack;
	nodeStack.push_back(&root);
	SerializedNode* node = nodeStack.back();

	int const SPACES_PER_TAB = 4;

	for (std::string line : lines)
	{
		// skip empty/whitespace
		if (line.size() == 0 || line.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
		{
			continue;
		}

		// count number of tabs (indents)
		int spaces = 0;
		for (char const c : line)
		{
			if (c == ' ' || c == '\t')
			{
				spaces++;
			}
			else
			{
				break;
			}
		}
		int i = spaces / SPACES_PER_TAB;

		int indentChange = i - indent;

		// if new indent is too deep, ignore
		if (indentChange > 1)
		{
			console::warn(std::format("Discarding line, invalid indent change of {}: {}", indentChange, line));
			continue;
		}

		// check change in index
		if (indentChange > 0)
		{
			// add last child to node stack
			nodeStack.push_back(&node->children.at(key));

			// start using that as active node
			node = nodeStack.back();

			// update indent
			indent = i;
		}
		else if (indentChange < 0)
		{
			// going down, so pop down X nodes, where X is the difference between indents
			for (; indentChange < 0; indentChange++)
			{
				nodeStack.pop_back();
			}

			// update node reference
			node = nodeStack.back();

			// update indent
			indent = i;
		}

		// remove indents for parsing
		if (spaces > 0)
		{
			line = line.substr(static_cast<size_t>(spaces), line.size() - spaces);
		}

		// split by colon, if there is one
		size_t split = line.find_first_of(':');

		if (split == std::string::npos)
		{
			// no split, just key
			key = line;

			// add as child
			node->children.emplace(key, SerializedNode());
		}
		else
		{
			// split: implies key: value
			key = line.substr(0, split);
			value = line.substr(split + 2, line.size() - split - 2); // ignore ": "

			// add as child
			node->children.emplace(key, SerializedNode{ .data = value });
		}
	}

	// root node should contain everything
	return root;
}