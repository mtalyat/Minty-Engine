#include "pch.h"
#include "Minty/Rendering/M_PixelFormat.h"

using namespace Minty;
using namespace Minty;

#include "Minty/Tools/M_Text.h"

using namespace Minty;

String Minty::to_string(PixelFormat const value)
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

PixelFormat Minty::from_string_texture_builder_pixel_format(String const& value)
{
    String value2 = Minty::Text::to_upper(value);
    if (value2 == "G") return PixelFormat::Gray;
    if (value2 == "GA") return PixelFormat::GrayAlpha;
    if (value2 == "RGB") return PixelFormat::RedGreenBlue;
    if (value2 == "RGBA") return PixelFormat::RedGreenBlueAlpha;

    return static_cast<PixelFormat>(0);
}
