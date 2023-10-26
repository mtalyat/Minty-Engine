#pragma once
#include "M_Object.h"
#include "M_Key.h"
#include "M_KeyAction.h"
#include "M_KeyModifiers.h"
#include "M_MouseButton.h"
#include "M_Event.h"
#include <map>
#include <unordered_set>

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

	std::string to_string(KeyPressEventArgs const& value);

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

	std::string to_string(MouseClickEventArgs const& value);

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

	std::string to_string(MouseMoveEventArgs const& value);

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

	std::string to_string(MouseScrollEventArgs const& value);

	/// <summary>
	/// Holds events for actions taken by the user.
	/// By putting all events into a map, the map can be easily swapped out,
	/// allowing for easy and dynamic alterations of controls.
	/// </summary>
	class InputMap :
		public Object
	{
	public:
		typedef Event<KeyPressEventArgs const&> KeyEvent_t;
		typedef Event<MouseClickEventArgs const&> ClickEvent_t;
		typedef Event<MouseMoveEventArgs const&> MoveEvent_t;
		typedef Event<MouseScrollEventArgs const&> ScrollEvent_t;

	private:
		// key press
		std::map<Key, KeyEvent_t>* _keyDownEvents;
		std::map<Key, KeyEvent_t>* _keyUpEvents;
		std::map<Key, KeyEvent_t>* _keyEvents;
		std::unordered_set<Key>* _keys;
		// mouse click
		std::map<MouseButton, ClickEvent_t>* _mouseDownEvents;
		std::map<MouseButton, ClickEvent_t>* _mouseUpEvents;
		std::map<MouseButton, ClickEvent_t>* _mouseEvents;
		std::unordered_set<MouseButton>* _buttons;
		// mouse move
		MoveEvent_t* _mouseMoveEvent;
		// mouse scroll
		ScrollEvent_t* _mouseScrollEvent;

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
		void emplace_key_down(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given key is released.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void emplace_key_up(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given key is pressed down, held, or released.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void emplace_key(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is pressed down.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplace_mouse_down(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is released.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplace_mouse_up(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the given mouse button is pressed down or  released.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void emplace_mouse(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the mouse cursor moves.
		/// </summary>
		/// <param name="func">The callback.</param>
		void emplace_mouse_move(MoveEvent_t::func const& func);

		/// <summary>
		/// Places a new callback to be invoked when the mouse scrolls.
		/// </summary>
		/// <param name="func">The callback.</param>
		void emplace_mouse_scroll(ScrollEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the key down event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void erase_key_down(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the key up event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void erase_key_up(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the key event.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="func">The callback.</param>
		void erase_key(Key const key, KeyEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse down event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void erase_mouse_down(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse up event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void erase_mouse_up(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse event.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <param name="func">The callback.</param>
		void erase_mouse(MouseButton const button, ClickEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse move event.
		/// </summary>
		/// <param name="func">The callback.</param>
		void erase_mouse_move(MoveEvent_t::func const& func);

		/// <summary>
		/// Removes a callback from the mouse move event.
		/// </summary>
		/// <param name="func">The callback.</param>
		void erase_mouse_scroll(ScrollEvent_t::func const& func);

		/// <summary>
		/// Invokes a key press event.
		/// </summary>
		/// <param name="args"></param>
		void invoke_key(KeyPressEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse click event.
		/// </summary>
		/// <param name="args"></param>
		void invoke_mouse_click(MouseClickEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse move event.
		/// </summary>
		/// <param name="args"></param>
		void invoke_mouse_move(MouseMoveEventArgs const& args) const;

		/// <summary>
		/// Invokes a mouse scroll event.
		/// </summary>
		/// <param name="args"></param>
		void invoke_mouse_scroll(MouseScrollEventArgs const& args) const;

		/// <summary>
		/// Checks if the given keyboard key is being held down.
		/// </summary>
		/// <param name="key">The key to check.</param>
		/// <returns>True if the key is actively being pressed.</returns>
		bool is_key_pressed(Key const key) const;

		/// <summary>
		/// Checks if the given mouse button is being held down.
		/// </summary>
		/// <param name="button">The button to check.</param>
		/// <returns>True if the button is actively being pressed.</returns>
		bool is_button_pressed(MouseButton const button) const;

		friend std::string to_string(InputMap const& value);
	};
}
