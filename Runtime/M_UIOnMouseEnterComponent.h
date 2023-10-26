#pragma once

#include "M_Component.h"
#include "M_Event.h"
#include "M_InputMap.h"

namespace minty
{
	struct UIOnMouseEnterComponent
		: public Component
	{
		InputMap::MoveEvent_t onMouseEnter;
	};
}