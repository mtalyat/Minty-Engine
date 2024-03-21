#include "pch.h"
#include "M_Animator.h"

#include "M_Console.h"

using namespace minty;

minty::Animator::Animator()
	: Asset()
	, _fsm()
{}

minty::Animator::Animator(AnimatorBuilder const& builder, Runtime& runtime)
	: Asset(builder.id, builder.path, runtime)
	, _fsm(builder.fsm)
{}

void minty::Animator::set_variable(String const& name, int const value)
{
	_fsm.set_variable(name, value);
}

int minty::Animator::get_variable(String const& name) const
{
	return _fsm.get_variable(name);
}

UUID minty::Animator::update()
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
		return INVALID_UUID;
	case 2:
		// infinite loop
		Console::warn(std::format("Animator::update(): Infinite loop indicated."));
		return INVALID_UUID;
	default:
		Console::error(std::format("Animator::update(): Unrecognized FSM fsmResult code {}", result));
		return INVALID_UUID;
	}

	Dynamic const& value = _fsm.get_current_value();

	if (value.size() == sizeof(UUID))
	{
		return value.get<UUID>();
	}
	else
	{
		Console::error(std::format("Animator::update(): FSM value was not the size of a UUID. sizeof(UUID): {}, size: {}", sizeof(UUID), value.size()));
	}

	// no state or value
	return INVALID_UUID;
}

FSM& minty::Animator::get_fsm()
{
	return _fsm;
}
