#pragma once
#include "Minty/Components/M_Component.h"

namespace Minty
{
	/// <summary>
	/// Used to mark when the Entity has been dirtied/changed.
	/// </summary>
	struct DirtyComponent
		: public Component
	{};
}