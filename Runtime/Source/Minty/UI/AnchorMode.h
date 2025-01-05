#pragma once

namespace Minty
{
	enum class AnchorMode : int
	{
		Empty = 0,
		All = 0b00000000,
		Top = 0b00000001,
		Middle = 0b00000010,
		Bottom = 0b00000100,
		Left = 0b00001000,
		Center = 0b00010000,
		Right = 0b00100000,
	};

	AnchorMode operator |(AnchorMode const left, AnchorMode const right);

	AnchorMode operator &(AnchorMode const left, AnchorMode const right);
}