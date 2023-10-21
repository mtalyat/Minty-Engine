#pragma once

#include <Minty.h>

namespace game
{
    class CameraControllerSystem
        : public minty::System
    {
    public:
    float pitch;    // x
    float yaw;      // y
    bool mouseDown;

    public:
        CameraControllerSystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void update() override;
    };
}