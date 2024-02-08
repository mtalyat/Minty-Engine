#include "pch.h"
#include "M_Encoding.h"

#include <vector>

using namespace minty;

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
String minty::Encoding::encode_base64(Dynamic const& in)
{
    String out;

    Byte* bytes = static_cast<Byte*>(in.data());
    size_t count = in.size();

    int val = 0, valb = -6;
    for (size_t i = 0; i < count; i++)
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
