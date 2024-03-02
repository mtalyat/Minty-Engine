#pragma once

#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Holds data for an Entity that can hear audio in the Scene.
	/// </summary>
	struct AudioListenerComponent
		: public Component
	{
		friend String to_string(AudioListenerComponent const& value);
	};
}