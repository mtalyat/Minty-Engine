#pragma once
#include "M_Event.h"

#include "Minty/Input/M_MouseButton.h"

namespace Minty
{
	class MouseMovedEvent
		: public Event
	{
	private:
		float _x;
		float _y;

	public:
		MouseMovedEvent(float const x, float const y)
			: _x(x), _y(y) {}

		float get_x() const { return _x; }
		float get_y() const { return _y; }

		MINTY_EVENT_CLASS_TYPE(MouseMoved)
	};

	class MouseScrolledEvent
		: public Event
	{
	private:
		float _x, _y;

	public:
		MouseScrolledEvent(float const deltaX, float const deltaY)
			: _x(deltaX), _y(deltaY) {}

		float get_delta_x() const { return _x; }
		float get_delta_y() const { return _y; }

		MINTY_EVENT_CLASS_TYPE(MouseScrolled)
	};

	class MouseButtonEvent
		: public Event
	{
	private:
		MouseButton _button;
		KeyAction _action;
		KeyModifiers _keyMods;

	public:
		MouseButtonEvent(MouseButton const button, KeyAction const action, KeyModifiers const mods)
			: _button(button), _action(action), _keyMods(mods) {}

		MouseButton get_button() const { return _button; }

		KeyAction get_action() const { return _action; }

		KeyModifiers get_key_modifiers() const { return _keyMods; }

		MINTY_EVENT_CLASS_TYPE(MouseButton)
	};
}