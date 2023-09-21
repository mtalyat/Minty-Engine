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
	/// <summary>
	/// Event arguments for when a key is pressed, held or released.
	/// </summary>
	struct KeyPressEventArgs
	{
		Key key;
		KeyAction action;
		KeyModifiers mods;
	};

	/// <summary>
	/// Event arguments for when a mouse button is pressed, held or released.
	/// </summary>
	struct MouseClickEventArgs
	{
		MouseButton button;
		KeyAction action;
		KeyModifiers mods;
		float x, y;
	};

	/// <summary>
	/// Event arguments for when the mouse is moved.
	/// </summary>
	struct MouseMoveEventArgs
	{
		/// <summary>
		/// The current X position of the mouse.
		/// </summary>
		float x;

		/// <summary>
		/// The current Y position of the mouse.
		/// </summary>
		float y;
		
		/// <summary>
		/// The change in X since the last frame.
		/// </summary>
		float dx;
		
		/// <summary>
		/// The change in Y since the last frame.
		/// </summary>
		float dy;
	};

	/// <summary>
	/// Event arguments for when the mouse scrolls.
	/// </summary>
	struct MouseScrollEventArgs
	{
		/// <summary>
		/// The amount the mouse has scrolled on the X axis.
		/// </summary>
		float dx;
		
		/// <summary>
		/// The amount the mouse has scrolled on the Y axis.
		/// </summary>
		float dy;
	};

	/// <summary>
	/// Holds events for actions taken by the user.
	/// By putting all events into a map, the map can be easily swapped out,
	/// allowing for easy and dynamic alterations of controls.
	/// </summary>
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
		/// <summary>
		/// Create a new InputMap.
		/// </summary>
		InputMap();

		~InputMap();

		/// <summary>
		/// Places a new callback to be invoked when the given key is pressed down.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void emplaceKeyDown(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given key is released.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void emplaceKeyUp(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given key is pressed down, held, or released.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void emplaceKey(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is pressed down.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplaceMouseDown(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is released.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplaceMouseUp(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is pressed down or  released.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplaceMouse(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the mouse cursor moves.
		/// </summary>
		/// <param name="func">The callback.</param>
		void emplaceMouseMove(move_event_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the mouse scrolls.
		/// </summary>
		/// <param name="func">The callback.</param>
		void emplaceMouseScroll(scroll_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the key down event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void eraseKeyDown(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the key up event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void eraseKeyUp(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the key event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void eraseKey(Key const key, key_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse down event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void eraseMouseDown(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse up event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void eraseMouseUp(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void eraseMouse(MouseButton const button, click_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse move event.
		/// </summary>
		/// <param name="func">The callback.</param>
		void eraseMouseMove(move_event_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse move event.
		/// </summary>
		/// <param name="func">The callback.</param>
		void eraseMouseScroll(scroll_event_t::func const& func);

		/// <summary>
		/// Invokes a key press event.
		/// </summary>
		/// <param name="args"></param>
		void invokeKey(KeyPressEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse click event.
		/// </summary>
		/// <param name="args"></param>
		void invokeMouseClick(MouseClickEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse move event.
		/// </summary>
		/// <param name="args"></param>
		void invokeMouseMove(MouseMoveEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse scroll event.
		/// </summary>
		/// <param name="args"></param>
		void invokeMouseScroll(MouseScrollEventArgs const& args) const;
	};
}
