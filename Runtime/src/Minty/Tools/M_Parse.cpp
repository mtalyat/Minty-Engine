#include "pch.h"
#include "Minty/Tools/M_Parse.h"

#include "Minty/Tools/M_Text.h"
#include "Minty/Tools/M_Encoding.h"

using namespace Minty;
using namespace Minty::Parse;

bool is_unsigned_integer(String const& string)
{
    if (!string.length()) return false; // nothing in string

    for (char const c : string)
    {
        if (!isdigit(c)) return false; // not a digit
    }

    // all digits
    return true;
}

bool is_signed_integer(String const& string)
{
    if (!string.length()) return false; // nothing in string

    size_t i = string.at(0) == '-' ? 1 : 0;

    // check if just the '-'
    if (i == 1 && string.length() == 1) return false;

    char c;
    for (; i < string.size(); i++)
    {
        c = string.at(i);

        if (!isdigit(c)) return false; // not a digit
    }

    // all digits or - at beginning
    return true;
}

bool is_float(String const& string)
{
    if (!string.length()) return false; // nothing in string

    char c;
    bool decimal = false;
    bool digit = false;
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
            digit = true;
            break;
        }
    }

    // can only be a number if there was a digit
    return digit;
}

bool Minty::Parse::to_bool(String const& string)
{
    // check for "true" or "false" as well
    String upper = Text::to_upper(string);
    if (upper == "TRUE")
    {
        return true;
    }
    else if (upper == "FALSE")
    {
        return false;
    }

    // by default, parse the int to a bool
    return static_cast<bool>(std::stoi(string));
}

bool Minty::Parse::try_bool(String const& string, bool& value)
{
    // check for "true" or "false" as well
    String upper = Text::to_upper(string);
    if (upper == "TRUE")
    {
        return true;
    }
    else if (upper == "FALSE")
    {
        return false;
    }

    // by default, parse the int to a bool
    if (is_signed_integer(string))
    {
        value = static_cast<bool>(std::stoi(string));
        return true;
    }

    return false;
}

Byte Minty::Parse::to_byte(String const& string)
{
    return static_cast<Byte>(std::stoi(string));
}

bool Minty::Parse::try_byte(String const& string, Byte& value)
{
    if (is_unsigned_integer(string))
    {
        value = static_cast<Byte>(std::stoi(string));
        return true;
    }

    return false;
}

short Minty::Parse::to_short(String const& string)
{
    return static_cast<short>(std::stoi(string));
}

bool Minty::Parse::try_short(String const& string, short& value)
{
    if (is_signed_integer(string))
    {
        value = static_cast<short>(std::stoi(string));
        return true;
    }

    return false;
}

int Minty::Parse::to_int(String const& string)
{
    return std::stoi(string);
}

bool Minty::Parse::try_int(String const& string, int& value)
{
    if (is_signed_integer(string))
    {
        value = std::stoi(string);
        return true;
    }

    return false;
}

UUID Minty::Parse::to_uuid(String const& string)
{
    std::stringstream ss(string);
    UUID id(INVALID_UUID);
    ss >> id;
    return id;
}

bool Minty::Parse::try_uuid(String const& string, UUID& value)
{
    // UUIDs stored as base16
    if (Encoding::is_base16(string))
    {
        value = to_uuid(string);
        return true;
    }

    return false;
}

ID Minty::Parse::to_id(String const& string)
{
    return to_int(string);
}

bool Minty::Parse::try_id(String const& string, ID& value)
{
    return try_int(string, value);
}

unsigned int Minty::Parse::to_uint(String const& string)
{
    return static_cast<unsigned int>(std::stoul(string));
}

bool Minty::Parse::try_uint(String const& string, unsigned int& value)
{
    if (is_unsigned_integer(string))
    {
        value = static_cast<unsigned int>(std::stoul(string));
        return true;
    }

    return false;
}

long Minty::Parse::to_long(String const& string)
{
    return std::stol(string);
}

bool Minty::Parse::try_long(String const& string, int& value)
{
    if (is_signed_integer(string))
    {
        value = std::stol (string);
        return true;
    }

    return false;
}

float Minty::Parse::to_float(String const& string)
{
    return std::stof(string);
}

bool Minty::Parse::try_float(String const& string, float& value)
{
    if (is_float(string))
    {
        value = std::stof(string);
        return true;
    }

    return false;
}

double Minty::Parse::to_double(String const& string)
{
    return std::stod(string);
}

bool Minty::Parse::try_double(String const& string, double& value)
{
    if (is_float(string))
    {
        value = std::stod(string);
        return true;
    }

    return false;
}

size_t Minty::Parse::to_size(String const& string)
{
    return std::stoull(string);
}

bool Minty::Parse::try_size(String const& string, size_t& value)
{
    if (is_unsigned_integer(string))
    {
        value = std::stoull(string);
        return true;
    }

    return false;
}
