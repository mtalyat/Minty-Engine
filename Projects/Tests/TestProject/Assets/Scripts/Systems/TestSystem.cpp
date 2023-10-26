#include "TestSystem.h"

#include "../Components/TestComponent.h"

using namespace minty;

game::TestSystem::TestSystem(minty::Engine *const engine, minty::EntityRegistry *const registry)
    : System::System(engine, registry)
{}

void game::TestSystem::update()
{
    float deltaTime = _engine->get_delta_time();

    // move all with test component
    for(auto&& [entity, test, transform] : _registry->view<TestComponent, TransformComponent>().each())
    {
        test.translation += test.translateSpeed * deltaTime;
        test.rotation += test.rotateSpeed * deltaTime;
        test.scale += test.scaleSpeed * deltaTime;

        transform.local.position.y = math::sin(test.translation);
        transform.local.rotation = Quaternion::from_euler_angles(0.0f, test.rotation, 0.0f);
        float s = math::cos(test.scale) * 0.25f + 0.75f;
        transform.local.scale = Vector3(s, s, s);
    }
}
