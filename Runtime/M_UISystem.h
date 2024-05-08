#pragma once
#include "M_System.h"

#include "M_Entity.h"
#include "M_InputMap.h"
#include "M_Vector.h"
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
        Vector2 _mousePosition;
        bool _mouseDown;
        bool _clicking;
        std::unordered_set<Entity> _family;

    public:
        UISystem(Runtime& engine, Scene& scene);

        void update() override;
    };
}