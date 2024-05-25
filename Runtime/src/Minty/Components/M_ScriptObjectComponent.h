#pragma once
#include "M_Component.h"

namespace Minty
{
	struct ScriptObjectComponent
		: public Component
	{
		UUID id;

		ScriptObjectComponent() = default;
		ScriptObjectComponent(UUID const id)
			: id(id) {}
	};
}