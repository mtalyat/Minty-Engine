#pragma once

#include <string>
#include <vector>

namespace minty::string
{
	/// <summary>
	/// Converts the given string to all lowercase characters.
	/// </summary>
	/// <param name="string"></param>
	/// <returns></returns>
	std::string to_lower(std::string const& string);

	/// <summary>
	/// Converts the given string to all uppercase characters.
	/// </summary>
	/// <param name="string"></param>
	/// <returns></returns>
	std::string to_upper(std::string const& string);

	/// <summary>
	/// Converts the given data to a string of byte number values, separated by spaces.
	/// </summary>
	/// <param name="data">A pointer to the data to be converted.</param>
	/// <param name="size">The number of bytes to convert to a string.</param>
	/// <returns>A string containing all of the bytes.</returns>
	std::string to_bytes(void* const data, size_t const size);

	/// <summary>
	/// Converts the given data to a string of binary number values, separated by spaces.
	/// </summary>
	/// <param name="data">A pointer to the data to be converted.</param>
	/// <param name="size">The number of bytes to convert to a string.</param>
	/// <returns>A string contining all of the bits.</returns>
	std::string to_bits(void* const data, size_t const size);
	
	/// <summary>
	/// Checks if the two strings are equal, disregarding case sensitivity.
	/// </summary>
	/// <param name="left">The first string.</param>
	/// <param name="right">The second string.</param>
	/// <returns>True if the strings are equal.</returns>
	bool equal_insensitive(std::string const& left, std::string const& right);

	/// <summary>
	/// Splits the string based on whitespace.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<std::string> split(std::string const& string);

	/// <summary>
	/// Splits the string based on the given delimiter.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <param name="delimiter">The character to split the string by.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<std::string> split(std::string const& string, char const delimiter = ' ');

	/// <summary>
	/// Splits the string based on the given delimiter.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <param name="delimiter">The string to split the string by.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<std::string> split(std::string const& string, std::string const& delimiter = " ");
}