#pragma once
#include "Minty/Core/Types.h"

#include <string>
#include <format>

namespace Minty
{
	using String = std::string;

	constexpr Char const* const EMPTY = "";

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
	String to_bytes(void const* const data, Size const size);

	/// <summary>
	/// Converts the given data to a string of binary number values, separated by spaces.
	/// </summary>
	/// <param name="data">A pointer to the data to be converted.</param>
	/// <param name="size">The number of bytes to convert to a string.</param>
	/// <returns>A string contining all of the bits.</returns>
	String to_bits(void const* const data, Size const size);

	/// <summary>
	/// Checks if the two strings are equal, disregarding case sensitivity.
	/// </summary>
	/// <param name="left">The first string.</param>
	/// <param name="right">The second string.</param>
	/// <returns>True if the strings are equal.</returns>
	Bool equal_insensitive(String const& left, String const& right);

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
	std::vector<String> split(String const& string, Char const delimiter);

	/// <summary>
	/// Splits the string based on the given delimiter.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <param name="delimiter">The character to split the string by.</param>
	/// <returns>A vector of strings, not including the delimiters.</returns>
	std::vector<String> split(String const& string, String const& delimiter);

	/// <summary>
	/// Splits the string based on one or more spaces in a row.
	/// </summary>
	/// <param name="string">The string to split.</param>
	/// <returns>A vector of strings, not including the spaces.</returns>
	std::vector<String> split_words(String const& string);

	String join(std::vector<String> const& list, String const& separator = ", ");

	String join(std::vector<String> const& list, Size const start, Size const count, String const& separator = ", ");

	String replace(String const& string, String const& oldText, String const& newText);

	// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	//void ltrim(std::string& s) {
	//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned Char ch) {
	//		return !std::isspace(ch);
	//		}));
	//}

	//// trim from end (in place)
	//void rtrim(std::string& s) {
	//	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned Char ch) {
	//		return !std::isspace(ch);
	//		}).base(), s.end());
	//}
}