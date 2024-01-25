#pragma once

#include "M_System.h"
#include "M_AnimatorComponent.h"
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
        Register<Animator> _animators;
        Register<Animation> _animations;

    public:
        AnimationSystem(Engine& engine, ID const sceneId);

        void update() override;

        void reset() override;

    public:
        ID create_animation(AnimationBuilder const& builder);

        ID create_animator(AnimatorBuilder const& builder);

        ID find_animation(String const& name) const;

        ID find_animator(String const& name) const;

        Animation& get_animation(ID const id);

        Animation const& get_animation(ID const id) const;

        Animator& get_animator(ID const id);

        Animator const& get_animator(ID const id) const;

        String get_animation_name(ID const id) const;

        String get_animator_name(ID const id) const;

        ID load_animation(Path const& path);

        ID load_animator(Path const& path);

        void destroy_animation(ID const id);

        void destroy_animator(ID const id);

    public:
        void serialize(Writer& writer) const override;
        void deserialize(Reader const& reader) override;
    };
}