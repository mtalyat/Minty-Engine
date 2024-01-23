#include "TestSystem.h"

#include "../Components/TestComponent.h"

using namespace minty;

game::TestSystem::TestSystem(Scene& scene)
    : System::System(scene)
{}

void game::TestSystem::update()
{
    if(_paused)
    {
        // do nothing if paused
        return;
    }

    float deltaTime = get_scene().get_engine().get_delta_time();

    EntityRegistry& registry = get_entity_registry();

    // move all with test component
    for(auto&& [entity, test, transform] : registry.view<TestComponent, TransformComponent>().each())
    {
        test.rotation += test.rotationSpeed * deltaTime;
        transform.local.position = Quaternion(test.rotation) * Vector3(0.0f, 0.0f, 10.0f);
        registry.emplace_or_replace<DirtyComponent>(entity);
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
    EntityRegistry& registry = get_entity_registry();

    for(auto&& [entity, test, transform] : registry.view<TestComponent, TransformComponent>().each())
    {
        test.rotation = Vector3();
        transform.local.rotation = Quaternion(test.rotation);
    }
}
