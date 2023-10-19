#pragma once

#include "M_Base.h"

namespace minty
{
	enum class CursorMode
	{
		Normal,
		Hidden,
		Disabled,
	};

	std::string to_string(CursorMode const mode);
}