#include "pch.h"
#include "M_SerializedNode.h"

#include "M_Console.h"
#include "M_File.h"
#include <vector>

using namespace minty;

bool minty::SerializedNode::try_get_int(int& value) const
{
	std::string temp = data;
	if (data.starts_with('-'))
	{
		temp = temp.substr(1, temp.size() - 1);
	}

	for (char const c : temp)
	{
		if (!isdigit(c))
		{
			// not a digit: not an int
			return false;
		}
	}

	value = std::stoi(data);
	return true;
}

void minty::SerializedNode::print(int const indent) const
{
	// print children
	// parent takes care of printing this node's data
	for (auto const& pair : children)
	{
		if (pair.second.data.size())
		{
			console::print(std::format("{}{}: {}", std::string(indent, '\t'), pair.first, pair.second.data));
		}
		else
		{
			console::print(std::format("{}{}", std::string(indent, '\t'), pair.first));
		}

		// if child has children, print those, recusrively
		pair.second.print(indent + 1);
	}
}


SerializedNode minty::SerializedNode::parse_file(std::string const& path)
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