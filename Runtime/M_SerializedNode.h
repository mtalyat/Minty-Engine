#pragma once

#include "M_Types.h"
#include <string>
#include <map>

namespace minty
{
	struct SerializedNode
	{
		std::string data;

		std::map <std::string, SerializedNode> children;

		void print(int const indent = 0) const;
	};
}