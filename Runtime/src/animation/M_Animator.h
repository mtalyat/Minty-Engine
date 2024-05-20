#pragma once
#include "assets/M_Asset.h"

#include "types/M_FiniteStateMachine.h"

namespace minty
{
	/// <summary>
	/// Holds data to create a new Animator.
	/// </summary>
	struct AnimatorBuilder
	{
		UUID id;

		Path path;

		/// <summary>
		/// The finite state machine within the Animator.
		/// </summary>
		FSM fsm;
	};

	class Animator
		: public Asset
	{
	private:
		FSM _fsm;

	public:
		Animator();

		Animator(AnimatorBuilder const& builder, Runtime& runtime);

		void set_variable(String const& name, int const value);

		int get_variable(String const& name) const;

		UUID update();

		FSM& get_fsm();
	};
}