#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// A button on the mouse.
	/// </summary>
	enum class MouseButton
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,
		Left = 0,
		Right = 1,
		Middle = 2
	};

	String to_string(MouseButton const value);
}