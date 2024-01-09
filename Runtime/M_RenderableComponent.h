#pragma once

#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Holds data for an Entity that is to be rendered to the screen. All Entites without this component will be invisible.
	/// </summary>
	struct RenderableComponent
		: public Component
	{
		friend String to_string(RenderableComponent const& value);
	};
}