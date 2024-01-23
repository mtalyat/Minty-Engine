#pragma once

#include "M_Object.h"
#include "M_FiniteStateMachine.h"
#include "M_AnimatorBuilder.h"

namespace minty
{
	class Animator
		: public Object
	{
	private:
		FSM _fsm;

	public:
		Animator();

		Animator(AnimatorBuilder const& builder);

		void set_variable(String const& name, int const value);

		ID update();

		FSM& get_fsm();
	};
}