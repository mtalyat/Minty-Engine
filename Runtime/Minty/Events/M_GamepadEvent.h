#pragma once
#include "Minty/Events/M_Event.h"

#include "Minty/Input/M_Gamepad.h"
#include "Minty/Input/M_KeyAction.h"

namespace Minty
{
	class GamepadEvent
		: public Event
	{
	private:
		int _controller;

	public:
		GamepadEvent(int const controller)
			: _controller(controller) {}

		int get_controller() const { return _controller; }
	};

	class GamepadConnectedEvent
		: public GamepadEvent
	{
	public:
		GamepadConnectedEvent(int const controller)
			: GamepadEvent(controller) {}

		MINTY_EVENT_CLASS_TYPE(GamepadConnected)
	};

	class GamepadDisconnectedEvent
		: public GamepadEvent
	{
	public:
		GamepadDisconnectedEvent(int const controller)
			: GamepadEvent(controller) {}

		MINTY_EVENT_CLASS_TYPE(GamepadDisconnected)
	};

	class GamepadButtonEvent
		: public GamepadEvent
	{
	private:
		GamepadButton _button;
		KeyAction _action;

	public:
		GamepadButtonEvent(int const controller, GamepadButton const button, KeyAction const action)
			: GamepadEvent(controller), _button(button), _action(action) {}

		GamepadButton get_button() const { return _button; }

		KeyAction get_action() const { return _action; }

		MINTY_EVENT_CLASS_TYPE(GamepadButton)
	};

	class GamepadAxisEvent
		: public GamepadEvent
	{
	private:
		GamepadAxis _axis;
		float _value;

	public:
		GamepadAxisEvent(int const controller, GamepadAxis const axis, float const value)
			: GamepadEvent(controller), _axis(axis), _value(value) {}

		GamepadAxis get_axis() const { return _axis; }

		float get_value() const { return _value; }

		MINTY_EVENT_CLASS_TYPE(GamepadAxis)
	};
}