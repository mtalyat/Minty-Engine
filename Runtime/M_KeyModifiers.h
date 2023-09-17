#pragma once

namespace minty
{
	enum class KeyModifiers
	{
		Shift =			0b000001,
		Control =		0b000010,
		Alt =			0b000100,
		Super =			0b001000,
		CapsLock =		0b010000,
		NumLock =		0b100000
	};
}