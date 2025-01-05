#pragma once
#include "Minty/Core/Types.h"
#include "Minty/Input/Gamepad.h"
#include "Minty/Input/Key.h"
#include "Minty/Input/Mouse.h"

namespace Minty
{
	enum class EventType
	{
		None = 0,

		WindowClose,
		WindowResize,
		//WindowFocus,
		//WindowUnfocus,
		//WindowMoved,

		Key,

		MouseButton,
		MouseMoved,
		MouseScrolled,

		GamepadConnected,
		GamepadDisconnected,
		GamepadButton,
		GamepadAxis,
	};

#define MINTY_EVENT_CLASS_TYPE(type) static EventType get_static_type() { return EventType::type; }\
virtual EventType get_event_type() const override { return get_static_type(); }\
virtual char const* get_name() const override { return #type; }

	class Event
	{
	public:
		Bool handled = false;

	public:
		virtual ~Event() = default;

		virtual EventType get_event_type() const = 0;
		virtual char const* get_name() const = 0;
	};

	class EventDispatcher
	{
	private:
		Event& _event;

	public:
		EventDispatcher(Event& event)
			: _event(event)
		{}

		template<typename T, typename F>
		Bool dispatch(F const& func)
		{
			if (_event.get_event_type() == T::get_static_type())
			{
				_event.handled |= func(static_cast<T&>(_event));
				return true;
			}

			return false;
		}
	};

	class WindowResizeEvent
		: public Event
	{
	private:
		unsigned int _width, _height;

	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: _width(width), _height(height) {}

		unsigned int get_width() const { return _width; }
		unsigned int get_height() const { return _height; }

		MINTY_EVENT_CLASS_TYPE(WindowResize)
	};

	class WindowCloseEvent
		: public Event
	{
	public:
		WindowCloseEvent() = default;

		MINTY_EVENT_CLASS_TYPE(WindowClose)
	};

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
		float m_value;

	public:
		GamepadAxisEvent(int const controller, GamepadAxis const axis, float const value)
			: GamepadEvent(controller), _axis(axis), m_value(value) {}

		GamepadAxis get_axis() const { return _axis; }

		float get_value() const { return m_value; }

		MINTY_EVENT_CLASS_TYPE(GamepadAxis)
	};
}