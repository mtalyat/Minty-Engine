#pragma once
#include "M_System.h"

#include "M_Entity.h"
#include "M_InputMap.h"
#include <unordered_set>

namespace minty
{
    class Runtime;

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
        UISystem(Runtime& engine, ID const sceneId);

        void load() override;

        void update() override;

        void unload() override;
    };
}