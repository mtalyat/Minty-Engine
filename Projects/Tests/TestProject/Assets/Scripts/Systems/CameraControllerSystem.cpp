#include "CameraControllerSystem.h"

using namespace minty;

game::CameraControllerSystem::CameraControllerSystem(Runtime& runtime, ID const sceneId)
    : System::System(runtime, sceneId)
    , pitch()
    , yaw()
    , mouseDown()
{
}

void game::CameraControllerSystem::update()
{
    // cap pitch
    pitch = Math::clamp(pitch, -89.0f, 89.0f);

    EntityRegistry& registry = get_entity_registry();

    // rotate cameras
    for (auto &&[entity, camera, transform] : registry.view<CameraComponent const, TransformComponent>().each())
    {
        Quaternion quat = Quaternion::from_euler_angles(Math::deg2rad(pitch), Math::deg2rad(yaw), 0.0f);
        if (transform.localRotation != quat)
        {
            transform.localRotation = quat;
            registry.emplace_or_replace<DirtyComponent>(entity);
        }
    }
}
