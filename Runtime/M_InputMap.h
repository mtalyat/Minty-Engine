#pragma once
#include "M_Object.h"
#include "M_Key.h"
#include "M_KeyAction.h"
#include "M_KeyModifiers.h"
#include "M_MouseButton.h"
#include "M_Event.h"
#include <map>

namespace minty
{
	struct KeyPressEventArgs
	{
		Key key;
		KeyAction action;
		KeyModifiers mods;
	};

	struct MouseClickEventArgs
	{
		MouseButton button;
		KeyAction action;
		KeyModifiers mods;
		float x, y;
	};

	struct MouseMoveEventArgs
	{
		float x, y;
		float dx, dy;
	};

	struct MouseScrollEventArgs
	{
		float dx, dy;
	};

	class InputMap :
		public Object
	{
	private:
		typedef Event<KeyPressEventArgs const&> key_event_t;
		typedef Event<MouseClickEventArgs const&> click_event_t;
		typedef Event<MouseMoveEventArgs const&> move_event_t;
		typedef Event<MouseScrollEventArgs const&> scroll_event_t;

		// key press
		std::map<Key, key_event_t>* _keyDownEvents;
		std::map<Key, key_event_t>* _keyUpEvents;
		std::map<Key, key_event_t>* _keyEvents;
		// mouse click
		std::map<MouseButton, click_event_t>* _mouseDownEvents;
		std::map<MouseButton, click_event_t>* _mouseUpEvents;
		std::map<MouseButton, click_event_t>* _mouseEvents;
		// mouse move
		move_event_t* _mouseMoveEvent;
		// mouse scroll
		scroll_event_t* _mouseScrollEvent;

	public:
		InputMap();

		~InputMap();

		void emplaceKeyDown(Key const key, key_event_t::func const& func);

		void emplaceKeyUp(Key const key, key_event_t::func const& func);

		void emplaceKey(Key const key, key_event_t::func const& func);

		void emplaceMouseDown(MouseButton const button, click_event_t::func const& func);

		void emplaceMouseUp(MouseButton const button, click_event_t::func const& func);

		void emplaceMouse(MouseButton const button, click_event_t::func const& func);

		void emplaceMouseMove(move_event_t::func const& func);

		void emplaceMouseScroll(scroll_event_t::func const& func);

		void eraseKeyDown(Key const key, key_event_t::func const& func);

		void eraseKeyUp(Key const key, key_event_t::func const& func);

		void eraseKey(Key const key, key_event_t::func const& func);

		void eraseMouseDown(MouseButton const button, click_event_t::func const& func);

		void eraseMouseUp(MouseButton const button, click_event_t::func const& func);

		void eraseMouse(MouseButton const button, click_event_t::func const& func);

		void eraseMouseMove(move_event_t::func const& func);

		void eraseMouseScroll(scroll_event_t::func const& func);

		void invokeKey(KeyPressEventArgs const& args) const;

		void invokeMouseClick(MouseClickEventArgs const& args) const;

		void invokeMouseMove(MouseMoveEventArgs const& args) const;

		void invokeMouseScroll(MouseScrollEventArgs const& args) const;
	};
}
