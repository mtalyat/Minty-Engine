#pragma once
#include "M_Component.h"

namespace minty
{
	struct OnPointerEventComponent
		: public Component
	{

	};

	struct OnPointerEnterEventComponent
		: public OnPointerEventComponent
	{

	};

	struct OnPointerExitEventComponent
		: public OnPointerEventComponent
	{

	};
}