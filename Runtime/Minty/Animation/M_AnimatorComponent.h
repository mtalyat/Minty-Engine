#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Animation/M_Animator.h"
#include "Minty/Animation/M_Animation.h"

namespace Minty
{
	/// <summary>
	/// Holds data for an animator within the Scene.
	/// </summary>
	struct AnimatorComponent
		: public Component
	{
		/// <summary>
		/// Handles controlling animation flow.
		/// </summary>
		Animator animator;

		/// <summary>
		/// The ID of the current animation being ran.
		/// </summary>
		Ref<Animation> animation = nullptr;

		/// <summary>
		/// The amount of time elapsed on the animation.
		/// </summary>
		float time = 0.0f;

		/// <summary>
		/// The step index of the current animation.
		/// </summary>
		Animation::Index index = 0;

		void reset();

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(AnimatorComponent const& value);
	};
}