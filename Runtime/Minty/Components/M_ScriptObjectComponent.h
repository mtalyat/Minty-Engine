#pragma once
#include "M_Component.h"

namespace Minty
{
	struct ScriptObjectComponent
		: public Component
	{
		UUID id;

		ScriptObjectComponent()
			: id(UUID::create()) {}

		ScriptObjectComponent(UUID const id)
			: id(id) {}
	};
}