#pragma once

#include "core/M_Base.h"

namespace minty
{
	/// <summary>
	/// A button on a gamepad.
	/// </summary>
	enum class GamepadButton
	{
		ButtonSouth = 0,
		ButtonEast = 1,
		ButtonWest = 2,
		ButtonNorth = 3,
		LeftBumper = 4,
		RightBumper = 5,
		Back = 6,
		Start = 7,
		Home = 8,
		LeftThumb = 9,
		RightThumb = 10,
		DpadUp = 11,
		DpadRight = 12,
		DpadDown = 13,
		DpadLeft = 14,
	};

	/// <summary>
	/// An axis on a gamepad.
	/// </summary>
	enum class GamepadAxis
	{
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5,
	};
}