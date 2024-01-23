#include "CameraControllerSystem.h"

using namespace minty;

game::CameraControllerSystem::CameraControllerSystem(Engine& engine, ID const sceneId)
    : System::System(engine, sceneId)
    , pitch()
    , yaw()
    , mouseDown()
{
}

void game::CameraControllerSystem::update()
{
    // cap pitch
    pitch = math::clamp(pitch, -89.0f, 89.0f);

    EntityRegistry& registry = get_entity_registry();

    // rotate cameras
    for (auto &&[entity, camera, transform] : registry.view<CameraComponent const, TransformComponent>().each())
    {
        Quaternion quat = Quaternion::from_euler_angles(math::deg2rad(pitch), math::deg2rad(yaw), 0.0f);
        if (transform.local.rotation != quat)
        {
            transform.local.rotation = quat;
            registry.emplace_or_replace<DirtyComponent>(entity);
        }
    }
}
