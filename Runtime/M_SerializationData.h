#pragma once

#include "M_EntityRegistry.h"
#include "M_Engine.h"
#include "M_Scene.h"
#include "M_Renderer.h"

namespace minty
{
	struct SerializationData
	{
		Scene* scene;
		Entity entity;
	};
}