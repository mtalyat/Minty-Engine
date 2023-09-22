#include "pch.h"
#include "M_SerializedNode.h"

#include "M_Console.h"

using namespace minty;

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
