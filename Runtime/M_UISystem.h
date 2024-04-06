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
    public:
        UISystem(Runtime& engine, Scene& scene);

        void update() override;
    };
}