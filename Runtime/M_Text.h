#pragma once

#include "M_Types.h"
#include <vector>

namespace minty::Text
{
	constexpr char const* const EMPTY = "";

	/// <summary>
	/// Converts the given string to all lowercase characters.
	/// </summary>
	/// <param name="string"></param>
	/// <returns></returns>
	String to_lower(String const& string);

	/// <summary>
	/// Converts the given string to all uppercase characters.
	/// </summary>
	/// <param name="string"></param>
	/// <returns></returns>
	String to_upper(String const& string);

	/// <summary>
	/// Converts the given data to a string of byte number values, separated by spaces.
	/// </summary>
	/// <param name="data">A pointer to the data to be converted.</param>
	/// <param name="size">The number of bytes to convert to a string.</param>
	/// <returns>A string containing all of the bytes.</returns>
	String to_bytes(void const* const data, size_t const size);

	/// <summary>
	/// Converts the given data to a string of binary number values, separated by spaces.
	/// </summary>
	/// <param name="data">A pointer to the data to be converted.</param>
	/// <param name="size">The number of bytes to convert to a string.</param>
	/// <returns>A string contining all of the bits.</returns>
	String to_bits(void const* const data, size_t const size);
	
	/// <summary>
	/// Checks if the two strings are equal, disregarding case sensitivity.
	/// </summary>
	/// <param name="left">The first string.</param>
	/// <param name="right">The second string.</param>
	/// <returns>True if the strings are equal.</returns>
	bool equal_insensitive(String const& left, String const& right);

	/// <summary>
	/// Splits the string based on whitespace.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<String> split(String const& string);

	/// <summary>
	/// Splits the string based on the given delimiter.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <param name="delimiter">The character to split the string by.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<String> split(String const& string, char const delimiter);

	/// <summary>
	/// Splits the string based on the given delimiter.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <param name="delimiter">The string to split the string by.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<String> split(String const& string, String const& delimiter);

	String join(std::vector<String> const& list);

	String join(std::vector<String> const& list, size_t start, size_t count);

	// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	//inline void ltrim(std::string& s) {
	//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
	//		return !std::isspace(ch);
	//		}));
	//}

	//// trim from end (in place)
	//inline void rtrim(std::string& s) {
	//	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
	//		return !std::isspace(ch);
	//		}).base(), s.end());
	//}
}