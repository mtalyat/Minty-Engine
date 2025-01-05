#pragma once

#include "Minty/Core/Type.h"

namespace Minty
{
	enum class Accessibility : Byte
	{
		None = 0,

		Private = 1 << 0,
		Internal = 1 << 1,
		Protected = 1 << 2,
		Public = 1 << 3,

		Any = 0b00001111,
	};

	inline Accessibility operator |(Accessibility const left, Accessibility const right)
	{
		return static_cast<Accessibility>(static_cast<Int>(left) | static_cast<Int>(right));
	}

	inline Accessibility operator &(Accessibility const left, Accessibility const right)
	{
		return static_cast<Accessibility>(static_cast<Int>(left) & static_cast<Int>(right));
	}
}