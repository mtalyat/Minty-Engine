#pragma once

#include "M_System.h"
#include "M_Engine.h"
#include <unordered_set>

namespace minty
{
    /// <summary>
    /// Handles User Interface (UI) inputs.
    /// </summary>
    class UISystem
        : public System
    {
    private:
        std::unordered_set<Entity> _hoverEntities;

        InputMap::MoveEvent_t::func _onMouseMove;
        InputMap::ClickEvent_t::func _onMouseClick;

    public:
        UISystem(Scene& scene);

        void load() override;

        void update() override;

        void unload() override;
    };
}