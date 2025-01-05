#pragma once
#include "System.h"

#include "Minty/Animation/Animator.h"
#include "Minty/Animation/Animation.h"

namespace Minty
{
    /// <summary>
    /// Handles User Interface (UI) inputs.
    /// </summary>
    class AnimationSystem
        : public System
    {
    public:
        AnimationSystem(Scene& scene)
            : System("Animation", scene)
        {}

        void update(Time const& time) override;
    };
}