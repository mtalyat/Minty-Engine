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
		Entity entity;

		/// <summary>
		/// Handles controlling animation flow.
		/// </summary>
		FSM<ID> fsm;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(AnimatorComponent const& value);
	};
}