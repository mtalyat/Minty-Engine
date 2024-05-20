#pragma once

#include "core/M_Base.h"
#include "libraries/M_EnTT.h"

namespace minty
{
	typedef entt::entity Entity;
	constexpr Entity NULL_ENTITY = entt::null;

	String to_string(Entity const value);
}