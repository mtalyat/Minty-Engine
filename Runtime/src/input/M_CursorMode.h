#pragma once

#include "core/M_Base.h"

namespace minty
{
	enum class CursorMode
	{
		Normal,
		Hidden,
		Disabled,
	};

	String to_string(CursorMode const mode);
}