#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// The state of a key or button.
	/// </summary>
	enum class KeyAction
	{
		Up = 0,
		Down = 1,
		Hold = 2
	};

	String to_string(KeyAction const value);
}