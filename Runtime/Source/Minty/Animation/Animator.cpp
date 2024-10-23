#include "pch.h"
#include "Animator.h"

using namespace Minty;

UUID Minty::Animator::update()
{
	Int result = m_fsm.evaluate();

	// an error occured
	switch (result)
	{
	case -1: // did not transition, same state
	case 0: // transitioned, new state
		break;
	case 1: // did nothing, no state
		MINTY_WARN_FORMAT("Animator::update(): No current state set.");
		return INVALID_UUID;
	case 2:
		// infinite loop
		MINTY_WARN_FORMAT("Animator::update(): Infinite execute indicated.");
		return INVALID_UUID;
	default:
		MINTY_ERROR_FORMAT("Animator::update(): Unrecognized FSM fsmResult code {}", result);
		return INVALID_UUID;
	}

	Variable const& value = m_fsm.get_current_value();
	return value.get<UUID>();
}

