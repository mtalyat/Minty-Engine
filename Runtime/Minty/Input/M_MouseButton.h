#pragma once

#include "Minty/Core/M_Base.h"

namespace Minty
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

		Left = Button1,
		Right = Button2,
		Middle = Button3,
	};

	String to_string(MouseButton const value);
}