#pragma once
#include "Minty/Component/Component.h"

namespace Minty
{
	/// <summary>
	/// Used to mark modified Entities.
	/// </summary>
	struct DirtyComponent
		: public Component
	{};
}
