#pragma once
#include "Minty/Systems/M_System.h"

#include "Minty/Animation/M_Animator.h"
#include "Minty/Animation/M_Animation.h"
#include "Minty/Types/M_Register.h"

namespace Minty
{
    /// <summary>
    /// Handles User Interface (UI) inputs.
    /// </summary>
    class AnimationSystem
        : public System
    {
    private:


    public:
        AnimationSystem(Scene& scene)
            : System("Animation", scene)
        {}

        void update(Time const time) override;

        void reset() override {}
    };
}