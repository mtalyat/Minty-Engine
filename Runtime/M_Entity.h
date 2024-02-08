#pragma once

#include "M_Base.h"
#include "libraries/entt/entt.hpp"

namespace minty
{
	typedef entt::entity Entity;
	constexpr Entity NULL_ENTITY = entt::null;

	String to_string(Entity const value);
}