#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/UUID.h"

namespace Minty
{
	struct ScriptObjectComponent
		: public Component
	{
		// ID to the script object that corresponds with this component
		UUID id = INVALID_UUID;

		virtual ~ScriptObjectComponent() = default;
	};
}