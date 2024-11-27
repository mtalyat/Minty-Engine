#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/UUID.h"

namespace Minty
{
	struct ScriptObjectComponent
		: public Component
	{
		/// <summary>
		/// ID to the script object that corresponds with this component.
		/// </summary>
		UUID id = INVALID_UUID;

		virtual ~ScriptObjectComponent() = default;
	};
}