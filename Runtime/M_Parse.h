#pragma once

#include "M_Types.h"
#include <string>

namespace minty::parse
{
	bool to_bool(std::string const& string);

	bool try_bool(std::string const& string, bool& value);

	byte to_byte(std::string const& string);

	bool try_byte(std::string const& string, byte& value);

	short to_short(std::string const& string);

	bool try_short(std::string const& string, short& value);

	int to_int(std::string const& string);

	bool try_int(std::string const& string, int& value);

	ID to_id(std::string const& string);

	bool try_id(std::string const& string, ID& value);

	unsigned int to_uint(std::string const& string);

	bool try_uint(std::string const& string, unsigned int& value);

	long to_long(std::string const& string);

	bool try_long(std::string const& string, int& value);

	float to_float(std::string const& string);

	bool try_float(std::string const& string, float& value);

	double to_double(std::string const& string);

	bool try_double(std::string const& string, double& value);

	size_t to_size_t(std::string const& string);

	bool try_size_t(std::string const& string, size_t& value);
}