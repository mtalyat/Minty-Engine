#include "pch.h"
#include "M_PixelCoordinateMode.h"

using namespace minty;

std::string minty::to_string(PixelCoordinateMode const value)
{
	switch (value)
	{
	case PixelCoordinateMode::Normalized: return "NORMALIZED";
	case PixelCoordinateMode::Pixel: return "PIXEL";
	default: return "";
	}
}

PixelCoordinateMode minty::from_string_pixel_coordinate_mode(std::string const& value)
{
	std::string value2 = minty::string::to_upper(value);
	if (value2 == "NORMALIZED") return PixelCoordinateMode::Normalized;
	if (value2 == "PIXEL") return PixelCoordinateMode::Pixel;

	return static_cast<PixelCoordinateMode>(0);
}
