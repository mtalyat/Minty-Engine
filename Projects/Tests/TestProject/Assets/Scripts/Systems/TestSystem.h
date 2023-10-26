#pragma once

#include <Minty.h>

namespace game
{
    class TestSystem
        : public minty::System
    {
    private:

    public:
        TestSystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void update() override;
    };
}