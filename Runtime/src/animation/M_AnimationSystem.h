#pragma once
#include "systems/M_System.h"

#include "animation/M_Animator.h"
#include "animation/M_Animation.h"
#include "types/M_Register.h"

namespace minty
{
    /// <summary>
    /// Handles User Interface (UI) inputs.
    /// </summary>
    class AnimationSystem
        : public System
    {
    private:


    public:
        AnimationSystem(Runtime& engine, Scene& scene);

        void update() override;

        void reset() override;
    };
}