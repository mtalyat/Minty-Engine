#pragma once

#include "M_Types.h"
#include "M_UUID.h"
#include <string>

namespace minty::Parse
{
	/// <summary>
	/// Parses the given string into a bool.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	bool to_bool(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a bool.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_bool(String const& string, bool& value);

	/// <summary>
	/// Parses the given string into a byte.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	Byte to_byte(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a byte.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_byte(String const& string, Byte& value);

	/// <summary>
	/// Parses the given string into a short.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	short to_short(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a short.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_short(String const& string, short& value);

	/// <summary>
	/// Parses the given string into an int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	int to_int(String const& string);

	/// <summary>
	/// Attempts to parse the given string into an int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_int(String const& string, int& value);

	/// <summary>
	/// Parses the given string into a UUID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	UUID to_uuid(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a UUID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_uuid(String const& string, UUID& value);

	/// <summary>
	/// Parses the given string into an ID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	ID to_id(String const& string);

	/// <summary>
	/// Attempts to parse the given string into an ID.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_id(String const& string, ID& value);

	/// <summary>
	/// Parses the given string into an unsigned int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	unsigned int to_uint(String const& string);

	/// <summary>
	/// Attempts to parse the given string into an unsigned int.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_uint(String const& string, unsigned int& value);

	/// <summary>
	/// Parses the given string into a long.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	long to_long(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a long.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_long(String const& string, int& value);

	/// <summary>
	/// Parses the given string into a float.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	float to_float(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a float.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_float(String const& string, float& value);

	/// <summary>
	/// Parses the given string into a double.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	double to_double(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a double.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_double(String const& string, double& value);

	/// <summary>
	/// Parses the given string into a size_t.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <returns>The parsed value.</returns>
	size_t to_size(String const& string);

	/// <summary>
	/// Attempts to parse the given string into a size_t.
	/// </summary>
	/// <param name="string">The string to parse.</param>
	/// <param name="value">The parsed value.</param>
	/// <returns>True if the parse was successful, otherwise false.</returns>
	bool try_size(String const& string, size_t& value);
}