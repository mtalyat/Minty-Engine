#pragma once

#include "M_System.h"
#include "M_Engine.h"
#include <unordered_set>

namespace minty
{
    class UISystem
        : public System
    {
    private:
        std::unordered_set<Entity> _hoverEntities;

        InputMap::MoveEvent_t::func _onMouseMove;
        InputMap::ClickEvent_t::func _onMouseClick;

    public:
        UISystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void load() override;

        void update() override;

        void unload() override;
    };
}