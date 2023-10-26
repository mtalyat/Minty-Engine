#pragma once

#include "M_Component.h"

namespace minty
{
	struct RenderableTag
		: public Component
	{
		friend std::string to_string(RenderableTag const& value);
	};
}