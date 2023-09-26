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

		byte to_byte(byte const defaultValue = 0) const;

		int to_int(int const defaultValue = 0) const;

		float to_float(float const defaultValue = 0.0f) const;

		void print(int const indent = 0) const;

		static SerializedNode parse_file(std::string const& path);
	};
}