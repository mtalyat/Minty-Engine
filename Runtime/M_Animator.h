#pragma once

#include "M_Object.h"
#include "M_FiniteStateMachine.h"

namespace minty
{
	class Animator
	{
	private:
		FSM<ID> _fsm;

	public:
		Animator();
	};
}