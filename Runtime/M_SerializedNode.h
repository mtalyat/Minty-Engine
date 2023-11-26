#pragma once

#include "M_Types.h"
#include <string>
#include <vector>

namespace minty
{
	struct Node
	{
		std::string data;

		std::vector <std::pair<std::string, Node>> children;

		byte to_byte(byte const defaultValue = 0) const;

		int to_int(int const defaultValue = 0) const;

		float to_float(float const defaultValue = 0.0f) const;

		Node* find(std::string const& name);

		Node const* find(std::string const& name) const;

		void print(int const indent = 0) const;

		static Node parse_file(std::string const& path);

	public:
		friend std::string to_string(Node const& value);
	};
}