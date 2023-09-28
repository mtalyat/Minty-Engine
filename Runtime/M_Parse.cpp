#include "pch.h"
#include "M_Parse.h"

using namespace minty;
using namespace minty::parse;

bool is_unsigned_integer(std::string const& string)
{
    if (!string.length()) return false; // nothing in string

    for (char const c : string)
    {
        if (!isdigit(c)) return false; // not a digit
    }

    // all digits
    return true;
}

bool is_signed_integer(std::string const& string)
{
    if (!string.length()) return false; // nothing in string

    char c;
    for (size_t i = 0; i < string.size(); i++)
    {
        c = string.at(i);

        switch (c)
        {
        case '-':
            if (i > 0) return false; // - not at beginning
            break;
        default:
            if (!isdigit(c)) return false; // not a digit
        }
    }

    // all digits or - at beginning
    return true;
}

bool is_float(std::string const& string)
{
    if (!string.length()) return false; // nothing in string

    char c;
    bool decimal = false;
    for (size_t i = 0; i < string.size(); i++)
    {
        c = string.at(i);

        switch (c)
        {
        case '-':
            if (i > 0) return false; // - not at beginning
            break;
        case '.':
            if (decimal) return false; // second .
            decimal = true;
            break;
        default:
            if (!isdigit(c)) return false; // not a digit
        }
    }

    return true;
}

bool minty::parse::to_bool(std::string const& string)
{
    return static_cast<bool>(std::stoi(string));
}

bool minty::parse::try_bool(std::string const& string, bool& value)
{
    if (is_signed_integer(string))
    {
        value = static_cast<bool>(std::stoi(string));
        return true;
    }

    return false;
}

byte minty::parse::to_byte(std::string const& string)
{
    return static_cast<byte>(std::stoi(string));
}

bool minty::parse::try_byte(std::string const& string, byte& value)
{
    if (is_unsigned_integer(string))
    {
        value = static_cast<byte>(std::stoi(string));
        return true;
    }

    return false;
}

short minty::parse::to_short(std::string const& string)
{
    return static_cast<short>(std::stoi(string));
}

bool minty::parse::try_short(std::string const& string, short& value)
{
    if (is_signed_integer(string))
    {
        value = static_cast<short>(std::stoi(string));
        return true;
    }

    return false;
}

int minty::parse::to_int(std::string const& string)
{
    return std::stoi(string);
}

bool minty::parse::try_int(std::string const& string, int& value)
{
    if (is_signed_integer(string))
    {
        value = std::stoi(string);
        return true;
    }

    return false;
}

unsigned int minty::parse::to_unsigned_int(std::string const& string)
{
    return static_cast<unsigned int>(std::stoul(string));
}

bool minty::parse::try_unsigned_int(std::string const& string, unsigned int& value)
{
    if (is_unsigned_integer(string))
    {
        value = static_cast<unsigned int>(std::stoul(string));
        return true;
    }

    return false;
}

long minty::parse::to_long(std::string const& string)
{
    return std::stol(string);
}

bool minty::parse::try_long(std::string const& string, int& value)
{
    if (is_signed_integer(string))
    {
        value = std::stol (string);
        return true;
    }

    return false;
}

float minty::parse::to_float(std::string const& string)
{
    return std::stof(string);
}

bool minty::parse::try_float(std::string const& string, float& value)
{
    if (is_float(string))
    {
        value = std::stof(string);
        return true;
    }

    return false;
}

double minty::parse::to_double(std::string const& string)
{
    return std::stod(string);
}

bool minty::parse::try_double(std::string const& string, double& value)
{
    if (is_float(string))
    {
        value = std::stod(string);
        return true;
    }

    return false;
}
