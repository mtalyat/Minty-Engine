#pragma once
#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Used to mark when the Entity should be destroyed at the end of the frame.
	/// </summary>
	struct DestroyComponent
		: Component
	{
		friend String to_string(DestroyComponent const& value);
	};
}