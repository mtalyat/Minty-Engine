#pragma once

#include <string>
#include <vector>

namespace minty::string
{
	std::string to_lower(std::string const& string);

	std::string to_upper(std::string const& string);

	std::string to_bytes(void* const data, size_t const size);

	std::string to_bits(void* const data, size_t const size);

	bool equal_insensitive(std::string const& left, std::string const& right);

	std::vector<std::string> split(std::string const& string);
	std::vector<std::string> split(std::string const& string, char const delimiter = ' ');
	std::vector<std::string> split(std::string const& string, std::string const& delimiter = " ");
}