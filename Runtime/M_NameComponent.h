#pragma once

#include "M_Types.h"
#include "M_Component.h"

namespace minty
{
	struct NameComponent
		: public Component
	{
		std::string name;
	};
}