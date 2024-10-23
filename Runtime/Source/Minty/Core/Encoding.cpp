#include "pch.h"
#include "Encoding.h"

#include <vector>
#include <algorithm>

using namespace Minty;

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
String Minty::Encoding::encode_base64(void const* const data, Size const size)
{
    String out;
    Byte const* bytes = static_cast<Byte const*>(data);

    Int val = 0, valb = -6;
    for (Size i = 0; i < size; i++)
    {
        Byte c = bytes[i];

        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

Bool Minty::Encoding::is_base64(String const& text)
{
    // from chatGPT:

    // Base64 strings must be divisible by 4
    if (text.empty() || text.size() % 4 != 0) {
        return false;
    }

    // Check each character to ensure it is part of the Base64 alphabet
    for (Char c : text) {
        if (!isalnum(c) && c != '+' && c != '/' && c != '=') {
            return false;
        }
    }

    // Check for proper padding
    Size paddingCount = 0;
    if (text.back() == '=') {
        paddingCount = 1;
        // Check for a second padding character
        if (text.size() > 1 && text[text.size() - 2] == '=') {
            paddingCount = 2;
        }
    }

    // More than two padding characters is invalid
    if (paddingCount > 2) {
        return false;
    }

    // Ensure padding, if present, is only at the end
    for (Size i = 0; i < text.size() - paddingCount; ++i) {
        if (text[i] == '=') {
            return false;
        }
    }

    return true;
}

String Minty::Encoding::encode_base16(void const* const data, Size const size)
{
    Byte const* bytes = reinterpret_cast<Byte const*>(data);

    std::stringstream ss;

    ss << std::hex << std::setfill('0');
    for (Size i = size - 1; i < size; i--)
    {
        ss << std::setw(2) << static_cast<Int>(bytes[i]);
    }

    return ss.str();
}

ConstantContainer Minty::Encoding::decode_base16(String const& string)
{
    Size size = string.size() >> 1;
    ConstantContainer container(size);
    std::vector<Byte> decodedBytes;

    for (std::size_t i = 0; i < string.size(); i += 2)
    {
        std::string byteString = string.substr(i, 2);
        Byte byte = static_cast<Byte>(std::stoi(byteString, nullptr, 16)); // Convert hex to byte
        decodedBytes.push_back(byte);
        container.set_at(&byte, sizeof(Byte), i >> 1);
    }

    return container;
}

Bool Minty::Encoding::is_base16(String const& text)
{
    if (text.empty()) return false;

    for (Char c : text)
    {
        if (!isdigit(c) && (c < 'a' || c > 'f') && (c < 'A' || c > 'F'))
        {
            // not a digit or a-f/A-F
            return false;
        }
    }

    return true;
}
