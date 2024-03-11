#pragma once
#include "M_System.h"

#include "M_Animator.h"
#include "M_Animation.h"
#include "M_Register.h"

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
    public:
        void deserialize(Reader const& reader) override;
    };
}