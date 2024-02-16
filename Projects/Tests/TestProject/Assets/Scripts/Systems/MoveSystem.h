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
        MoveSystem(minty::Runtime& runtime, minty::ID const sceneId);

        void update() override;
    };
}