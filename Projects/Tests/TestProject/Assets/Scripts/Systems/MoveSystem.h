#pragma once

#include <Minty.h>

namespace game
{
    class MoveSystem
        : public minty::System
    {
    private:

    public:
        float right;    // x
        float up;       // y
        float forward;  // z
        bool faster;

    public:
        MoveSystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void update() override;
    };
}