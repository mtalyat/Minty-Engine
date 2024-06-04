#pragma once
#include "M_Event.h"

namespace Minty
{
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
}