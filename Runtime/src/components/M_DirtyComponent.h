#pragma once
#include "components/M_Component.h"

namespace minty
{
	/// <summary>
	/// Used to mark when the Entity has been dirtied/changed.
	/// </summary>
	struct DirtyComponent
		: public Component
	{
		friend String to_string(DirtyComponent const& value);
	};
}