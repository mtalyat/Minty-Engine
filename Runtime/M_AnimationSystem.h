#pragma once

#include "M_System.h"
#include "M_AnimatorComponent.h"
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
        Register<Animation> _animations;

    public:
        AnimationSystem(minty::Engine* const engine, minty::EntityRegistry* const registry);

        void update() override;

    private:
        void update_animation(AnimatorComponent& animator, Animation const& animation) const;

    public:
        ID create_animation(AnimationBuilder const& builder);

        ID find_animation(String const& name) const;

        Animation& get_animation(ID const id);

        Animation const& get_animation(ID const id) const;

        String get_animation_name(ID const id) const;

        ID load_animation(Path const& path);

        void destroy_animation(ID const id);
    };
}