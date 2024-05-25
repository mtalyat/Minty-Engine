#include "pch.h"
#include "Minty/Rendering/M_RenderSystem.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Assets/M_Asset.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Rendering/M_Builtin.h"
#include "Minty/Types/M_Vector.h"
#include "Minty/Libraries/M_GLM.hpp"

#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Rendering/M_CameraComponent.h"
#include "Minty/Components/M_TransformComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Rendering/M_DrawCallObjectInfo.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;
using namespace Minty;
using namespace Minty::vk;
using namespace Minty::Builtin;

void Minty::RenderSystem::update(Time const time)
{
	// do nothing if no camera
	if (_camera == NULL_ENTITY)
	{
		MINTY_WARN("There is no Camera to render to!");
		return;
	}

	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();

	// update camera in renderer if it is enabled
	if (entityRegistry.all_of<EnabledComponent>(_camera))
	{
		CameraComponent const& camera = entityRegistry.get<CameraComponent>(_camera);
		TransformComponent const& transformComponent = entityRegistry.get<TransformComponent>(_camera);

		update_camera(camera, transformComponent);
	}
}

void Minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	RenderEngine& renderer = RenderEngine::instance();
	renderer.set_camera(transform.get_global_position(), transform.get_global_rotation(), camera.camera);
}
