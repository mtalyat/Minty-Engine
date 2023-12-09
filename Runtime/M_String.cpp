#include "pch.h"
#include "M_String.h"

#include "M_Console.h"

#include <sstream>

std::string minty::string::to_lower(std::string const& string)
{
    std::vector<char> data(string.size());

    for (size_t i = 0; i < string.size(); i++)
    {
        data[i] = std::tolower(string.at(i));
    }

    data.push_back('\0');

    return std::string(data.data());
}

std::string minty::string::to_upper(std::string const& string)
{
    std::vector<char> data(string.size());

    for (size_t i = 0; i < string.size(); i++)
    {
        data[i] = std::toupper(string.at(i));
    }

    data.push_back('\0');

    return std::string(data.data());
}

bool minty::string::equal_insensitive(std::string const& left, std::string const& right)
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

std::vector<std::string> minty::string::split(std::string const& string)
{
    std::stringstream ss(string);

    std::vector<std::string> results;
    std::string token;

    while (ss >> token)
    {
        results.push_back(token);
    }

    return results;
}

std::vector<std::string> minty::string::split(std::string const& string, char const delimiter)
{
    std::stringstream ss(string);

    std::vector<std::string> results;
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        results.push_back(token);
    }

    return results;
}

std::vector<std::string> minty::string::split(std::string const& string, std::string const& delimiter)
{
    console::todo("string::split() (string delimiter)");
    return {};
}
