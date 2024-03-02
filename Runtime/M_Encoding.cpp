#include "pch.h"
#include "M_Encoding.h"

#include <vector>

using namespace minty;

String minty::Encoding::encode_base64(Dynamic const& in)
{
    return encode_base64(in.data(), in.size());
}

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
String minty::Encoding::encode_base64(void const* const data, size_t const size)
{
    String out;
    Byte const* bytes = static_cast<Byte const*>(data);

    int val = 0, valb = -6;
    for (size_t i = 0; i < size; i++)
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

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
Dynamic minty::Encoding::decode_base64(String const& in)
{
    std::vector<Byte> bytes;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (Byte c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            bytes.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return Dynamic(bytes.data(), bytes.size());
}

bool minty::Encoding::is_base64(String const& text)
{
    // from chatGPT:

    // Base64 strings must be divisible by 4
    if (text.empty() || text.size() % 4 != 0) {
        return false;
    }

    // Check each character to ensure it is part of the Base64 alphabet
    for (char c : text) {
        if (!isalnum(c) && c != '+' && c != '/' && c != '=') {
            return false;
        }
    }

    // Check for proper padding
    size_t paddingCount = 0;
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
    for (size_t i = 0; i < text.size() - paddingCount; ++i) {
        if (text[i] == '=') {
            return false;
        }
    }

    return true;
}

String minty::Encoding::encode_base16(Dynamic const& data)
{
    return encode_base16(data.data(), data.size());
}

String minty::Encoding::encode_base16(void const* const data, size_t const size)
{
    Byte const* bytes = reinterpret_cast<Byte const*>(data);

    std::stringstream ss;

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < size; i++)
    {
        ss << std::setw(2) << static_cast<int>(bytes[i]);
    }

    return ss.str();
}

Dynamic minty::Encoding::decode_base16(String const& text)
{
    std::vector<Byte> bytes;

    std::stringstream ss(text);

    ss >> std::hex;
    Byte byte;
    for (size_t i = 0; i < text.size(); i += 2)
    {
        ss >> std::setw(2) >> byte;
        bytes.push_back(byte);
    }

    return Dynamic(bytes.data(), bytes.size());
}

bool minty::Encoding::is_base16(String const& text)
{
    if (text.empty()) return false;

    for (char c : text)
    {
        if (!isdigit(c) && (c < 'a' || c > 'f') && (c < 'A' || c > 'F'))
        {
            // not a digit or a-f/A-F
            return false;
        }
    }

    return true;
}
