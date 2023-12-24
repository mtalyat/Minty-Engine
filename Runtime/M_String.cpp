#include "pch.h"
#include "M_String.h"

#include "M_Console.h"

#include <sstream>

using namespace minty;

String minty::string::to_lower(String const& string)
{
	std::vector<char> data(string.size());

	for (size_t i = 0; i < string.size(); i++)
	{
		data[i] = std::tolower(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String minty::string::to_upper(String const& string)
{
	std::vector<char> data(string.size());

	for (size_t i = 0; i < string.size(); i++)
	{
		data[i] = std::toupper(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String minty::string::to_bytes(void* const data, size_t const size)
{
	String result = "";

	Byte* bytes = static_cast<Byte*>(data);

	for (size_t i = size; i-- > 0;)
	{
		result += std::to_string(bytes[i]);

		if (i)
		{
			result += " ";
		}
	}

	return result;
}

String minty::string::to_bits(void* const data, size_t const size)
{
	String result = "";

	Byte* bytes = static_cast<Byte*>(data);

	for (size_t i = size; i-- > 0;)
	{
		for (size_t j = 8; j-- > 0;)
		{
			result += std::to_string((bytes[i] >> j) & 1);
		}

		if (i)
		{
			result += " ";
		}
	}

	return result;
}

bool minty::string::equal_insensitive(String const& left, String const& right)
{
	// check if not equal lengths
	if (left.size() != right.size())
	{
		return false;
	}

	// check each character
	size_t length = left.size();
	for (size_t i = 0; i < length; i++)
	{
		if (std::toupper(left.at(i)) != std::toupper(right.at(i)))
		{
			return false;
		}
	}

	// must be equal if length and chars match
	return true;
}

std::vector<String> minty::string::split(String const& string)
{
	std::stringstream ss(string);

	std::vector<String> results;
	String token;

	while (ss >> token)
	{
		results.push_back(token);
	}

	return results;
}

std::vector<String> minty::string::split(String const& string, char const delimiter)
{
	std::stringstream ss(string);

	std::vector<String> results;
	String token;

	while (std::getline(ss, token, delimiter))
	{
		results.push_back(token);
	}

	return results;
}

std::vector<String> minty::string::split(String const& string, String const& delimiter)
{
	console::todo("string::split() (string delimiter)");
	return {};
}
