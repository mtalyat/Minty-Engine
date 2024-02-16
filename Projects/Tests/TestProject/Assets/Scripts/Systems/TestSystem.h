#pragma once

#include <Minty.h>

namespace game
{
    class TestSystem
        : public minty::System
    {
    private:
        bool _paused = false;

    public:
        TestSystem(minty::Runtime& runtime, minty::ID const sceneId);

        void update() override;

        void set_pause(bool const paused);

        void toggle_pause();

        // resets all test components
        void reset();
    };
}