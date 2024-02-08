#include "pch.h"
#include "M_PixelFormat.h"

using namespace minty;
using namespace minty;

#include "M_String.h"

using namespace minty;

String minty::to_string(PixelFormat const value)
{
    switch (value)
    {
    case PixelFormat::Gray: return "G";
    case PixelFormat::GrayAlpha: return "GA";
    case PixelFormat::RedGreenBlue: return "RGB";
    case PixelFormat::RedGreenBlueAlpha: return "RGBA";
    default: return "";
    }
}

PixelFormat minty::from_string_texture_builder_pixel_format(String const& value)
{
    String value2 = minty::Text::to_upper(value);
    if (value2 == "G") return PixelFormat::Gray;
    if (value2 == "GA") return PixelFormat::GrayAlpha;
    if (value2 == "RGB") return PixelFormat::RedGreenBlue;
    if (value2 == "RGBA") return PixelFormat::RedGreenBlueAlpha;

    return static_cast<PixelFormat>(0);
}
