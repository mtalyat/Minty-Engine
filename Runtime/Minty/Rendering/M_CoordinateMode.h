#pragma once

#include "Minty/Core/M_Base.h"

namespace Minty
{
	enum class CoordinateMode
	{
		/// <summary>
		/// The minimum, maximum and pivot will all be from [0.0, 1.0] on both the X and Y axes.
		/// </summary>
		Normalized,

		/// <summary>
		/// The minimum, maximum and pivot will all be from [0.0, width] on the X axis, and [0.0, height] on the Y axis.
		/// </summary>
		Pixel
	};

	std::string to_string(CoordinateMode const value);
	CoordinateMode from_string_coordinate_mode(std::string const& value);
}