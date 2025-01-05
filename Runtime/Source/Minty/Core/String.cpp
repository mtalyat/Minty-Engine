#include "pch.h"
#include "String.h"

#include "Minty/Core/Console.h"

#include <sstream>

using namespace Minty;

String Minty::to_lower(String const& string)
{
	std::vector<Char> data(string.size());

	for (Size i = 0; i < string.size(); i++)
	{
		data[i] = std::tolower(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String Minty::to_upper(String const& string)
{
	std::vector<Char> data(string.size());

	for (Size i = 0; i < string.size(); i++)
	{
		data[i] = std::toupper(string.at(i));
	}

	data.push_back('\0');

	return String(data.data());
}

String Minty::to_bytes(void const* const data, Size const size)
{
	String result = "";

	Byte const* bytes = static_cast<Byte const*>(data);

	for (Size i = size; i-- > 0;)
	{
		result += std::to_string(bytes[i]);

		if (i)
		{
			result += " ";
		}
	}

	return result;
}

String Minty::to_bits(void const* const data, Size const size)
{
	String result = "";

	Byte const* bytes = static_cast<Byte const*>(data);

	for (Size i = size; i-- > 0;)
	{
		for (Size j = 8; j-- > 0;)
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

Bool Minty::equal_insensitive(String const& left, String const& right)
{
	// check if not equal lengths
	if (left.size() != right.size())
	{
		return false;
	}

	// check each character
	Size length = left.size();
	for (Size i = 0; i < length; i++)
	{
		if (std::toupper(left.at(i)) != std::toupper(right.at(i)))
		{
			return false;
		}
	}

	// must be equal if length and chars match
	return true;
}

std::vector<String> Minty::split(String const& string)
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

std::vector<String> Minty::split(String const& string, Char const delimiter)
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

std::vector<String> Minty::split(String const& string, String const& delimiter)
{
	std::vector<String> results;
	String text = string;

	// add segments between each delimiter
	Size pos = 0;
	String token;
	while ((pos = text.find(delimiter)) != std::string::npos)
	{
		token = text.substr(0, pos);
		results.push_back(token);
		text.erase(0, pos + delimiter.length());
	}
	
	// add the rest of the text
	if (!text.empty())
	{
		results.push_back(text);
	}

	return results;
}

std::vector<String> Minty::split_words(String const& string)
{
	std::istringstream iss(string);
	return std::vector<std::string>{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

String Minty::join(std::vector<String> const& list, String const& separator)
{
	return join(list, 0, list.size(), separator);
}

String Minty::join(std::vector<String> const& list, Size const start, Size const count, String const& separator)
{
	String out;

	Size end = start + count;

	for (Size i = start; i < end; i++)
	{
		out += list.at(i);

		if (i < end - 1)
		{
			out += separator;
		}
	}

	return out;
}

String Minty::replace(String const& string, String const& oldText, String const& newText)
{
	String result = string;
	String::size_type pos = 0;

	// replace all instances of oldText with newText
	while ((pos = result.find(oldText, pos)) != String::npos) {
		result.replace(pos, oldText.length(), newText);
		pos += newText.length();
	}

	return result;
}
