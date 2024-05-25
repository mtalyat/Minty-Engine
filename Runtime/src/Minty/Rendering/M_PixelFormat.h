#pragma once

#include "Minty/Types/M_Types.h"

namespace Minty
{
	/// <summary>
	/// The format for how pixels are read.
	/// </summary>
	enum class PixelFormat
	{
		/// <summary>
		/// No format.
		/// </summary>
		None = 0,
		/// <summary>
		/// Gray channel only.
		/// </summary>
		Gray = 1,
		/// <summary>
		/// Gray and alpha channels.
		/// </summary>
		GrayAlpha = 2,
		/// <summary>
		/// Red, green and blue channels.
		/// </summary>
		RedGreenBlue = 3,
		/// <summary>
		/// Red, green, blue and alpha channels.
		/// </summary>
		RedGreenBlueAlpha = 4,
	};

	Minty::String to_string(PixelFormat const value);
	PixelFormat from_string_texture_builder_pixel_format(Minty::String const& value);
}