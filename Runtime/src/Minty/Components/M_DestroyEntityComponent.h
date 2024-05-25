#pragma once
#include "Minty/Components/M_Component.h"

namespace Minty
{
	/// <summary>
	/// Used to mark when the Entity should be destroyed at the end of the frame.
	/// </summary>
	struct DestroyEntityComponent
		: Component
	{};
}