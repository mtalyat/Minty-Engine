#pragma once

#include "Minty/Core/Types.h"

namespace Minty
{
	/// <summary>
	/// Holds data for a character within a font.
	/// </summary>
	struct FontChar
	{
		Char id;
		Float x;
		Float y;
		Float width;
		Float height;
		Float xOffset;
		Float yOffset;
		Float xAdvance;
	};
}