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
	case -1: // did not transition, same state
	case 0: // transitioned, new state
	case 1: // did nothing, no state
		break; // ok result
	case 2:
		// infinite loop
		console::warn(std::format("Animator::update(): Infinite loop indicated."));
		return ERROR_ID;
	default:
		console::error(std::format("Animator::update(): Unrecognized FSM fsmResult code {}", result));
		return ERROR_ID;
	}

	// if current state, and state has a value...
	if (_fsm.has_current_state())
	{
		Dynamic const& value = _fsm.get_current_value();

		if (value.size() == sizeof(ID))
		{
			return value.get<ID>();
		}
		else
		{
			console::error(std::format("Animator::update(): FSM value was not the size of an ID. sizeof(ID): {}, size: {}", sizeof(ID), value.size()));
		}
	}

	// no state or value
	return ERROR_ID;
}

FSM& minty::Animator::get_fsm()
{
	return _fsm;
}
