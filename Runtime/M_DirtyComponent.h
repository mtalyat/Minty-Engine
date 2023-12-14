#pragma once

#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Used to mark when the Entity has been dirtied/updated.
	/// </summary>
	struct DirtyComponent
		: public Component
	{
		friend std::string to_string(DirtyComponent const& value);
	};
}