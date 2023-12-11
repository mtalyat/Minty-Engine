#include "pch.h"
#include "M_Rendering_TextureBuilder.h"

using namespace minty;
using namespace minty::rendering;

std::string minty::rendering::to_string(TextureBuilder::PixelFormat const value)
{
    switch (value)
    {
    case TextureBuilder::PixelFormat::Gray: return "G";
    case TextureBuilder::PixelFormat::GrayAlpha: return "GA";
    case TextureBuilder::PixelFormat::RedGreenBlue: return "RGB";
    case TextureBuilder::PixelFormat::RedGreenBlueAlpha: return "RGBA";
    default: return "";
    }
}

TextureBuilder::PixelFormat minty::rendering::from_string_texture_builder_pixel_format(std::string const& value)
{
    std::string value2 = minty::string::to_upper(value);
    if (value2 == "G") return TextureBuilder::PixelFormat::Gray;
    if (value2 == "GA") return TextureBuilder::PixelFormat::GrayAlpha;
    if (value2 == "RGB") return TextureBuilder::PixelFormat::RedGreenBlue;
    if (value2 == "RGBA") return TextureBuilder::PixelFormat::RedGreenBlueAlpha;

    return static_cast<TextureBuilder::PixelFormat>(0);
}
