#include "pch.h"
#include "rendering/M_RenderSystem.h"

#include "rendering/M_RenderEngine.h"
#include "assets/M_Asset.h"
#include "runtime/M_Runtime.h"
#include "assets/M_AssetEngine.h"
#include "rendering/M_Builtin.h"
#include "types/M_Vector.h"
#include "libraries/M_GLM.hpp"

#include "entities/M_EntityRegistry.h"
#include "rendering/M_CameraComponent.h"
#include "components/M_TransformComponent.h"
#include "components/M_EnabledComponent.h"
#include "rendering/M_DrawCallObjectInfo.h"

#include "serialization/M_Reader.h"
#include "serialization/M_Writer.h"

using namespace minty;
using namespace minty;
using namespace minty::vk;
using namespace minty::Builtin;

minty::RenderSystem::RenderSystem(Runtime& engine, Scene& scene)
	: System::System("Render", engine, scene)
{}

minty::RenderSystem::~RenderSystem()
{}

void minty::RenderSystem::reset()
{ }

void minty::RenderSystem::update()
{
	// do nothing if no camera
	if (_mainCamera == NULL_ENTITY)
	{
		MINTY_WARN("There is no Camera to render to!");
		return;
	}

	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();

	// update camera in renderer if it is enabled
	if (entityRegistry.all_of<EnabledComponent>(_mainCamera))
	{
		CameraComponent const& camera = entityRegistry.get<CameraComponent>(_mainCamera);
		TransformComponent const& transformComponent = entityRegistry.get<TransformComponent>(_mainCamera);

		update_camera(camera, transformComponent);
	}
}

void minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	RenderEngine& renderer = get_runtime().get_render_engine();
	renderer.set_camera(transform.get_global_position(), transform.get_global_rotation(), camera.camera);
}

void minty::RenderSystem::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}
