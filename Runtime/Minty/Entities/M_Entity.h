#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Libraries/M_EnTT.h"

namespace Minty
{
	/// <summary>
	/// Represents an Entity within the game engine.
	/// </summary>
	typedef entt::entity Entity;

	/// <summary>
	/// An empty Entity value.
	/// </summary>
	constexpr Entity NULL_ENTITY = entt::null;
}