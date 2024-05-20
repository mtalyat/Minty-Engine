#pragma once
#include "components/M_Component.h"

namespace minty
{
	/// <summary>
	/// Used to mark when the Entity should be destroyed at the end of the frame.
	/// </summary>
	struct DestroyEntityComponent
		: Component
	{
		friend String to_string(DestroyEntityComponent const& value);
	};
}