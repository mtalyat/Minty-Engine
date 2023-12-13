#include "TestSystem.h"

#include "../Components/TestComponent.h"

using namespace minty;

game::TestSystem::TestSystem(minty::Engine *const engine, minty::EntityRegistry *const registry)
    : System::System(engine, registry)
{}

void game::TestSystem::update()
{
    if(_paused)
    {
        // do nothing if paused
        return;
    }

    float deltaTime = _engine->get_delta_time();

    // move all with test component
    for(auto&& [entity, test, transform] : _registry->view<TestComponent, TransformComponent>().each())
    {
        test.rotation += test.rotationSpeed * deltaTime;
        transform.local.position = Quaternion(test.rotation) * Vector3(0.0f, 0.0f, 10.0f);
        _registry->emplace_or_replace<Dirty>(entity);
    }
}

void game::TestSystem::set_pause(bool const paused)
{
    _paused = paused;
}

void game::TestSystem::toggle_pause()
{
    _paused = !_paused;
}

void game::TestSystem::reset()
{
    for(auto&& [entity, test, transform] : _registry->view<TestComponent, TransformComponent>().each())
    {
        test.rotation = Vector3();
        transform.local.rotation = Quaternion(test.rotation);
    }
}
