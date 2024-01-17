#pragma once

#include "M_Component.h"
#include "M_FiniteStateMachine.h"
#include "M_EntityRegistry.h"

namespace minty
{
	/// <summary>
	/// Holds data for an animator within the Scene.
	/// </summary>
	struct AnimatorComponent
		: public Component
	{
		/// <summary>
		/// The Entity with the SpriteComponent.
		/// </summary>
		Entity entity = NULL_ENTITY;

		/// <summary>
		/// Handles controlling animation flow.
		/// </summary>
		FSM<ID> fsm;

		/// <summary>
		/// The ID of the current animation being ran.
		/// </summary>
		ID current = ERROR_ID;

		/// <summary>
		/// The amount of time left on the current frame.
		/// </summary>
		float time = 0.0f;

		/// <summary>
		/// The frame index of the current animation.
		/// </summary>
		size_t index = 0;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(AnimatorComponent const& value);
	};
}