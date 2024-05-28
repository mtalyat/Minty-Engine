#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Entities/M_Entity.h"

namespace Minty
{
	class Scene;

	struct SerializationData
	{
		Scene* scene;
		Entity entity;
	};
}