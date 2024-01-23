#include "pch.h"
#include "M_Animator.h"

#include "M_Console.h"

using namespace minty;

minty::Animator::Animator()
	: _fsm()
{}

minty::Animator::Animator(AnimatorBuilder const& builder)
	: _fsm(builder.fsm)
{}

void minty::Animator::set_variable(String const& name, int const value)
{
	_fsm.set_variable("name", value);
}

ID minty::Animator::update()
{
	int result = _fsm.evaluate();

	// an error occured
	switch (result)
	{
	case 0: // transitioned
	case 1: // did not transition
		break; // ok result
	case 2:
		// no current state
		return ERROR_ID;
	case 3:
		// infinite loop
		console::warn(std::format("Infinite loop indicated in AnimatorComponent."));
		return ERROR_ID;
	default:
		console::error(std::format("Unrecognized FSM fsmResult code in AnimationSystem: {}", result));
		return ERROR_ID;
	}

	return _fsm.get_current_value().get<ID>();
}

FSM& minty::Animator::get_fsm()
{
	return _fsm;
}
