#pragma once

#include "Minty/Library/EnTT.h"

namespace Minty
{
	/// <summary>
	/// Represents an Entity within the game engine.
	/// </summary>
	using Entity = entt::entity;

	/// <summary>
	/// An empty Entity value.
	/// </summary>
	constexpr Entity NULL_ENTITY = entt::null;
}