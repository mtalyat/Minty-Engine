#pragma once

#include <Minty.h>

namespace game
{
    class TestSystem
        : public minty::System
    {
    private:
        bool _paused = true;

    public:
        TestSystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void update() override;

        void set_pause(bool const paused);

        void toggle_pause();

        // resets all test components
        void reset();
    };
}