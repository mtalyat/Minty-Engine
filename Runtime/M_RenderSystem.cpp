#include "pch.h"
#include "M_RenderSystem.h"

#include "M_RenderEngine.h"
#include "M_Asset.h"
#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_Builtin.h"
#include "M_Vector.h"
#include "M_GLM.hpp"

#include "M_EntityRegistry.h"
#include "M_CameraComponent.h"
#include "M_TransformComponent.h"
#include "M_EnabledComponent.h"
#include "M_DrawCallObjectInfo.h"

#include "M_Reader.h"
#include "M_Writer.h"

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
		Console::warn("There is no Camera to render to!");
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
