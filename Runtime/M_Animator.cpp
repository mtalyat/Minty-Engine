#include "pch.h"
#include "M_Animator.h"

#include "M_AnimatorBuilder.h"
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
	_fsm.set_variable(name, value);
}

int minty::Animator::get_variable(String const& name) const
{
	return _fsm.get_variable(name);
}

ID minty::Animator::update()
{
	int result = _fsm.evaluate();

	// an error occured
	switch (result)
	{
	case -1: // did not transition, same state
	case 0: // transitioned, new state
		break;
	case 1: // did nothing, no state
		Console::warn(std::format("Animator::update(): No current state set."));
		return ERROR_ID;
	case 2:
		// infinite loop
		Console::warn(std::format("Animator::update(): Infinite loop indicated."));
		return ERROR_ID;
	default:
		Console::error(std::format("Animator::update(): Unrecognized FSM fsmResult code {}", result));
		return ERROR_ID;
	}

	Dynamic const& value = _fsm.get_current_value();

	if (value.size() == sizeof(ID))
	{
		return value.get<ID>();
	}
	else
	{
		Console::error(std::format("Animator::update(): FSM value was not the size of an ID. sizeof(ID): {}, size: {}", sizeof(ID), value.size()));
	}

	// no state or value
	return ERROR_ID;
}

FSM& minty::Animator::get_fsm()
{
	return _fsm;
}
