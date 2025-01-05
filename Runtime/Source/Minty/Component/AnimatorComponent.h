#pragma once
#include "Component.h"

#include "Minty/Animation/Animator.h"
#include "Minty/Animation/Animation.h"

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
		Ref<Animator> animator = nullptr;

		/// <summary>
		/// The ID of the current animation being ran.
		/// </summary>
		Ref<Animation> animation = nullptr;

		/// <summary>
		/// The amount of time elapsed on the animation.
		/// </summary>
		Float time = 0.0f;

		/// <summary>
		/// The step index of the current animation.
		/// </summary>
		Animation::Index index = 0;

		void reset();

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}