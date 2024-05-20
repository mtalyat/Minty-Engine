#pragma once

#include "types/M_Types.h"

namespace minty::Scripting
{
	enum class Accessibility : uint8_t
	{
		None = 0,

		Private = 1 << 0,
		Internal = 1 << 1,
		Protected = 1 << 2,
		Public = 1 << 3,

		Any = Private | Internal | Protected | Public,
	};

	Accessibility operator |(Accessibility const left, Accessibility const right);
	Accessibility operator &(Accessibility const left, Accessibility const right);
}