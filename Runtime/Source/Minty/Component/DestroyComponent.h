#pragma once
#include "Minty/Component/Component.h"

namespace Minty
{
	/// <summary>
	/// Used to mark Entities that should be destroyed soon.
	/// </summary>
	struct DestroyComponent
		: public Component
	{};
}
