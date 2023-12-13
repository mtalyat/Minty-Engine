#include "MoveSystem.h"

#include "../Components/MoveComponent.h"

using namespace minty;

game::MoveSystem::MoveSystem(minty::Engine *const engine, minty::EntityRegistry *const registry)
    : System::System(engine, registry), right(), up(), forward(), faster()
{
}

void game::MoveSystem::update()
{
    // calculate move speed
    float const MOVE_SPEED = 1.0f;
    float movement = MOVE_SPEED * _engine->get_delta_time();

    // sprint multiplier
    if (faster)
    {
        movement *= 10.0f;
    }

    // m,
    for (auto &&[entity, move, transform] : _registry->view<MoveComponent const, TransformComponent>().each())
    {
        bool moved = false;
        if (right != 0.0f)
        {
            transform.local.position += transform.local.rotation.right() * movement * right;
            moved = true;
        }
        if (up != 0.0f)
        {
            transform.local.position += transform.local.rotation.up() * movement * up;
            moved = true;
        }
        if (forward != 0.0f)
        {
            transform.local.position += transform.local.rotation.forward() * movement * forward;
            moved = true;
        }
        if (moved)
        {
            _registry->emplace_or_replace<Dirty>(entity);
        }
    }
}
