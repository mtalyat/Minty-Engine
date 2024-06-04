#include "pch.h"
#include "Minty/Rendering/M_CoordinateMode.h"

#include "Minty/Tools/M_Text.h"

using namespace Minty;

std::string Minty::to_string(CoordinateMode const value)
{
	switch (value)
	{
	case CoordinateMode::Normalized: return "NORMALIZED";
	case CoordinateMode::Pixel: return "PIXEL";
	default: return "";
	}
}

CoordinateMode Minty::from_string_coordinate_mode(std::string const& value)
{
	std::string value2 = Minty::Text::to_upper(value);
	if (value2 == "NORMALIZED") return CoordinateMode::Normalized;
	if (value2 == "PIXEL") return CoordinateMode::Pixel;

	return static_cast<CoordinateMode>(0);
}
