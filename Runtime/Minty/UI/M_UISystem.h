#pragma once
#include "Minty/Systems/M_System.h"

#include "Minty/Entities/M_Entity.h"
#include "Minty/Types/M_Vector.h"
#include <unordered_set>

namespace Minty
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
        UISystem(Scene& scene)
            : System("UI", scene), _mousePosition(), _mouseDown(), _clicking(), _family() {}

        void update(Time const time) override;
    };
}