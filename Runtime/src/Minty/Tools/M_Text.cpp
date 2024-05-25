#include "pch.h"
#include "M_Text.h"

#include "Minty/Tools/M_Console.h"

#include <sstream>

using namespace Minty;

String Minty::Text::to_lower(String const& string)
{
	std::vector<char> data(string.size());

	for (size_t i = 0; i < string.size(); i++)
	{
		data[i] = std::tolower(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String Minty::Text::to_upper(String const& string)
{
	std::vector<char> data(string.size());

	for (size_t i = 0; i < string.size(); i++)
	{
		data[i] = std::toupper(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String Minty::Text::to_bytes(void const* const data, size_t const size)
{
	String result = "";

	Byte const* bytes = static_cast<Byte const*>(data);

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

String Minty::Text::to_bits(void const* const data, size_t const size)
{
	String result = "";

	Byte const* bytes = static_cast<Byte const*>(data);

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

bool Minty::Text::equal_insensitive(String const& left, String const& right)
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

std::vector<String> Text::split(String const& string)
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

std::vector<String> Text::split(String const& string, char const delimiter)
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

std::vector<String> Minty::Text::split(String const& string, String const& delimiter)
{
	std::vector<String> results;
	String text = string;

	size_t pos = 0;
	String token;
	while ((pos = text.find(delimiter)) != std::string::npos)
	{
		token = text.substr(0, pos);
		results.push_back(token);
		text.erase(0, pos + delimiter.length());
	}

	return results;
}

String Minty::Text::join(std::vector<String> const& list, String const& separator)
{
	return join(list, 0, list.size(), separator);
}

String Minty::Text::join(std::vector<String> const& list, size_t const start, size_t const count, String const& separator)
{
	String out;

	size_t end = start + count;

	for (size_t i = start; i < end; i++)
	{
		out += list.at(i);

		if (i < end - 1)
		{
			out += separator;
		}
	}

	return out;
}
