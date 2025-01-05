#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Entity/Entity.h"

namespace Minty
{
	class Scene;

	struct EntitySerializationData
	{
		Scene* scene;
		Entity entity;
	};
}