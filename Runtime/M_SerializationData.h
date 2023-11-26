#pragma once

#include "M_EntityRegistry.h"

namespace minty
{
	class Renderer;
	class Scene;

	struct SerializationData
	{
		Renderer* renderer;
		Scene* scene;
		Entity entity;
	};
}