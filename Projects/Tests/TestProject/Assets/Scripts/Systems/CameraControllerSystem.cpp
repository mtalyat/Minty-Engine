#include "CameraControllerSystem.h"

using namespace minty;

game::CameraControllerSystem::CameraControllerSystem(minty::Engine *const engine, minty::EntityRegistry *const registry)
    : System::System(engine, registry)
    , pitch()
    , yaw()
    , mouseDown()
{}

void game::CameraControllerSystem::update()
{
    // cap pitch
    pitch = math::clamp(pitch, -89.0f, 89.0f);

    // rotate cameras
    for(auto&& [entity, camera, transform] : _registry->view<CameraComponent const, TransformComponent>().each())
    {
        transform.local.rotation = Quaternion::from_euler_angles(math::deg2rad(pitch), math::deg2rad(yaw), 0.0f);
    }
}