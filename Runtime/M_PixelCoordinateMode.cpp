#include "pch.h"
#include "M_CoordinateMode.h"

using namespace minty;

std::string minty::to_string(CoordinateMode const value)
{
	switch (value)
	{
	case CoordinateMode::Normalized: return "NORMALIZED";
	case CoordinateMode::Pixel: return "PIXEL";
	default: return "";
	}
}

CoordinateMode minty::from_string_coordinate_mode(std::string const& value)
{
	std::string value2 = minty::Text::to_upper(value);
	if (value2 == "NORMALIZED") return CoordinateMode::Normalized;
	if (value2 == "PIXEL") return CoordinateMode::Pixel;

	return static_cast<CoordinateMode>(0);
}
