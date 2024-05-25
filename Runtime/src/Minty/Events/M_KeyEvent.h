#pragma once
#include "M_Event.h"

#include "Minty/Input/M_Key.h"
#include "Minty/Input/M_KeyAction.h"
#include "Minty/Input/M_KeyModifiers.h"

namespace Minty
{
	class KeyEvent
		: public Event
	{
	protected:
		Key _key;
		KeyAction _action;
		KeyModifiers _keyMods;

	public:
		KeyEvent(Key const key, KeyAction const action, KeyModifiers const mods)
			: _key(key), _action(action), _keyMods(mods) {}

		Key get_key() const { return _key; }

		KeyAction get_key_action() const { return _action; }

		KeyModifiers get_key_modifiers() const { return _keyMods; }

		MINTY_EVENT_CLASS_TYPE(Key)
	};
}