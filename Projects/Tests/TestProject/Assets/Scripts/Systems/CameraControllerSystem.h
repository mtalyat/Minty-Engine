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
        CameraControllerSystem(minty::Runtime& runtime, minty::ID const sceneId);

        void update() override;
    };
}