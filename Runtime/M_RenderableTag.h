#pragma once

#include "M_Component.h"

namespace minty
{
	struct Renderable
		: public Component
	{
		friend std::string to_string(Renderable const& value);
	};
}