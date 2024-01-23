#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// Holds data to create a new Animator.
	/// </summary>
	struct AnimatorBuilder
	{
		/// <summary>
		/// The name of the Animator.
		/// </summary>
		String name;

		/// <summary>
		/// The finite state machine within the Animator.
		/// </summary>
		FSM fsm;
	};
}