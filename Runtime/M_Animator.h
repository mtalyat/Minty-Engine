#pragma once

#include "M_Object.h"
#include "M_FiniteStateMachine.h"

namespace minty
{
	struct AnimatorBuilder;

	class Animator
		: public Object
	{
	private:
		FSM _fsm;

	public:
		Animator();

		Animator(AnimatorBuilder const& builder);

		void set_variable(String const& name, int const value);

		int get_variable(String const& name) const;

		ID update();

		FSM& get_fsm();
	};
}