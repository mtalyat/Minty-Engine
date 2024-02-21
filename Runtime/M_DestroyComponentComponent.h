#pragma once
#include "M_Component.h"

#include <unordered_set>

namespace minty
{
	struct DestroyComponentComponent
		: public Component
	{
		std::unordered_set<String> components;
	};
}