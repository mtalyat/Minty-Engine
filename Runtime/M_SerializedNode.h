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

		bool try_get_int(int& value) const;

		void print(int const indent = 0) const;

		static SerializedNode parse_file(std::string const& path);
	};
}