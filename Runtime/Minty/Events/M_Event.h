#pragma once
#include "Minty/Core/M_Base.h"

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
		bool handled = false;

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
		bool dispatch(F const& func)
		{
			if (_event.get_event_type() == T::get_static_type())
			{
				_event.handled |= func(static_cast<T&>(_event));
				return true;
			}

			return false;
		}
	};
}