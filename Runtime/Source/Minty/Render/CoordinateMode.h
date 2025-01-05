#pragma once

namespace Minty
{
	enum class CoordinateMode
	{
		/// <summary>
		/// Each coordinate is represented with a float that falls within the range of [0, 1].
		/// </summary>
		Normalized,

		/// <summary>
		/// Each coordinate is represented with an integer that falls within the range of [0, size], where size is the width or height, respectively.
		/// </summary>
		Pixel
	};
}