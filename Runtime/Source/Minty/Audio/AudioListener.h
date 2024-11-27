#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	struct AudioListener
	{
		Float3 position = Float3(0.0f, 0.0f, 0.0f);
		Float3 forward = Float3(0.0f, 0.0f, 1.0f); // forward vector
		Float3 up = Float3(0.0f, -1.0f, 0.0f); // up vector
		Float3 velocity = Float3(0.0f, 0.0f, 0.0f);
	};
}