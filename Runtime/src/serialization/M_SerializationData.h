#pragma once

#include "core/M_Base.h"
#include "entities/M_Entity.h"

namespace minty
{
	class Scene;

	struct SerializationData
	{
		Scene* scene;
		Entity entity;
	};
}