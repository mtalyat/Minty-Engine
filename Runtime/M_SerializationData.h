#pragma once

#include "M_Base.h"
#include "M_Entity.h"

namespace minty
{
	class Scene;

	struct SerializationData
	{
		Scene* scene;
		Entity entity;
	};
}