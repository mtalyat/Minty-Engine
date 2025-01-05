#pragma once

#include "Minty/Audio/Attenuation.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	struct AudioSource
	{
		Float3 position = Float3(0.0f, 0.0f, 0.0f);
		Float3 velocity = Float3(0.0f, 0.0f, 0.0f);
		Attenuation attenuation = Attenuation::LinearDistance;
		Float attenuationRollOff = 1.0f;
		Float minDistance = 1.0f;
		Float maxDistance = 10000.0f;
	};
}