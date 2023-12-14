#pragma once

#include "M_Types.h"
#include <string>

namespace minty::parse
{
	/// <summary>
	/// Parses the given string into a bool.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	bool to_bool(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a bool.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_bool(std::string const& string, bool& value);

	/// <summary>
	/// Parses the given string into a byte.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	byte to_byte(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a byte.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_byte(std::string const& string, byte& value);

	/// <summary>
	/// Parses the given string into a short.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	short to_short(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a short.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_short(std::string const& string, short& value);

	/// <summary>
	/// Parses the given string into an int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	int to_int(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into an int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_int(std::string const& string, int& value);

	/// <summary>
	/// Parses the given string into an ID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	ID to_id(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into an ID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_id(std::string const& string, ID& value);

	/// <summary>
	/// Parses the given string into an unsigned int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	unsigned int to_uint(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into an unsigned int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_uint(std::string const& string, unsigned int& value);

	/// <summary>
	/// Parses the given string into a long.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	long to_long(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a long.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_long(std::string const& string, int& value);

	/// <summary>
	/// Parses the given string into a float.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	float to_float(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a float.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_float(std::string const& string, float& value);

	/// <summary>
	/// Parses the given string into a double.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	double to_double(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a double.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_double(std::string const& string, double& value);

	/// <summary>
	/// Parses the given string into a size_t.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	size_t to_size(std::string const& string);

	/// <summary>
	/// Attempts to parse the given string into a size_t.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_size(std::string const& string, size_t& value);
}