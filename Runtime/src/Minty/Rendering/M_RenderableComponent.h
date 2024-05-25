#pragma once

#include "Minty/Components/M_Component.h"

namespace Minty
{
	/// <summary>
	/// Holds data for an Entity that is to be rendered to the screen. All Entites without this component will be invisible.
	/// </summary>
	struct RenderableComponent
		: public Component
	{};
}