#include "pch.h"
#include "M_RendererSystem.h"

#include "M_Engine.h"
#include "M_Renderer.h"
#include "M_CameraComponent.h"
#include "M_PositionComponent.h"
#include "M_RotationComponent.h"
#include "M_Vector3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>

using namespace minty;

minty::RendererSystem::RendererSystem(Engine* const engine, EntityRegistry* const registry)
	: System(engine, registry)
	, _renderer(&engine->get_renderer())
	, _mainCamera(NULL_ENTITY)
{}

void minty::RendererSystem::load()
{}

void minty::RendererSystem::update()
{
	// do nothing if no renderer
	if (!_renderer)
	{
		console::error("There is no Renderer to render to!");
		return;
	}

	// do nothing if no camera
	if (_mainCamera == NULL_ENTITY)
	{
		console::warn("There is no Camera to render to!");
		return;
	}

	// get camera transform
	CameraComponent& camera = _registry->get<CameraComponent>(_mainCamera);
	
	// get position if it exists
	Vector3 position;
	PositionComponent* positionComponent = _registry->try_get<PositionComponent>(_mainCamera);
	if (positionComponent)
	{
		position.x = positionComponent->x;
		position.y = positionComponent->y;
		position.z = positionComponent->z;
	}

	// get rotation if it exists
	Vector3 rotation;
	RotationComponent* rotationComponent = _registry->try_get<RotationComponent>(_mainCamera);
	if (rotationComponent)
	{
		rotation.x = rotationComponent->x;
		rotation.y = rotationComponent->y;
		rotation.z = rotationComponent->z;
	}

	// update camera in renderer
	_renderer->update_camera(camera, position, rotation);
}

void minty::RendererSystem::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}
