#pragma once
#include "types/M_Object.h"

#include "input/M_Key.h"
#include "input/M_KeyAction.h"
#include "input/M_KeyModifiers.h"
#include "input/M_MouseButton.h"
#include "input/M_CursorMode.h"
#include "input/M_Gamepad.h"
#include "types/M_Vector.h"
#include "types/M_RectF.h"
#include <unordered_map>

struct GLFWwindow;
struct GLFWgamepadstate;

namespace minty
{
	class ScriptClass;
	class ScriptEngine;

	/// <summary>
	/// A window on the screen.
	/// </summary>
	class Window
		: public Object
	{
	private:
		struct Gamepad
		{
			GLFWgamepadstate* state;

			char const* name;
		};

	private:
		static int _windowCount;

		String _title;
		GLFWwindow* _window;
		int _frameWidth, _frameHeight;
		int _width, _height;
		int _restoreX, _restoreY;
		bool _resized;

		ScriptClass const* _windowScript;
		ScriptClass const* _inputScript;

		Vector2 _mousePosition;
		bool _mouseDown;

		std::unordered_map<int, Gamepad> _gamepads;
	public:
		/// <summary>
		/// Creates a new Window.
		/// </summary>
		/// <param name="title">The title to be displayed on the Window.</param>
		/// <param name="width">The width of the Window in pixels.</param>
		/// <param name="height">The height of the Window in pixels.</param>
		Window(String const& title, int const width, int const height, Path const& iconPath = "");

		/// <summary>
		/// Creates a new Window.
		/// </summary>
		/// <param name="title"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		Window(String const& title, int const x, int const y, int const width, int const height, Path const& iconPath = "");

		~Window();

	public:
		void on_link(ScriptEngine& engine);

	public:
		/// <summary>
		/// Sets the title text of this Window.
		/// </summary>
		void set_title(String const& title);

		/// <summary>
		/// Gets the title text of this Window.
		/// </summary>
		String const& get_title() const;

		/// <summary>
		/// Sets the icon of this Window.
		/// </summary>
		/// <param name="path"></param>
		void set_icon(Path const& path);

		/// <summary>
		/// Sets the cursor mode.
		/// </summary>
		/// <param name="mode"></param>
		void set_cursor_mode(CursorMode const mode);

		/// <summary>
		/// Gets the cursor mode.
		/// </summary>
		/// <returns></returns>
		CursorMode get_cursor_mode() const;

		/// <summary>
		/// Maximizes the Window.
		/// </summary>
		void maximize();

		/// <summary>
		/// Minimizes the Window.
		/// </summary>
		void minimize();

		/// <summary>
		/// Restores the maximized or minimized window to its normal state.
		/// </summary>
		void restore();

		/// <summary>
		/// Checks if the Window has resized since the last time this method was called.
		/// </summary>
		/// <returns>True if the Window has been resized.</returns>
		bool is_resized();

		/// <summary>
		/// Checks if the Window is currently open
		/// </summary>
		/// <returns>True if the Window is not closed.</returns>
		bool is_open() const;

		/// <summary>
		/// Closes the window.
		/// </summary>
		void close();

		/// <summary>
		/// Refreshes the window's size.
		/// </summary>
		void refresh();

		/// <summary>
		/// Gets the framebuffer width of this Window.
		/// </summary>
		/// <returns></returns>
		int get_frame_width() const;

		/// <summary>
		/// Gets the framebuffer height of this Window.
		/// </summary>
		/// <returns></returns>
		int get_frame_height() const;

		/// <summary>
		/// Gets the width of this Window.
		/// </summary>
		/// <returns></returns>
		int get_width() const;

		/// <summary>
		/// Gets the height of this Window.
		/// </summary>
		/// <returns></returns>
		int get_height() const;

		/// <summary>
		/// Gets the position of the mouse in Window coordinates.
		/// </summary>
		/// <returns></returns>
		Vector2 get_mouse_position() const;

		/// <summary>
		/// Gets the position of the mouse, relative to the given bounds.
		/// </summary>
		/// <param name="bounds"></param>
		/// <returns></returns>
		Vector2 get_mouse_position(RectF const bounds) const;

		/// <summary>
		/// Gets if the mouse is currently being held down.
		/// </summary>
		bool get_mouse_down() const;

		/// <summary>
		/// Gets the raw GLFWwindow component.
		/// </summary>
		/// <returns>The raw window.</returns>
		GLFWwindow* get_raw() const;

		/// <summary>
		/// Processes all pending Window events.
		/// </summary>
		void poll_events();
	private:
		void save_restore_info();

		// triggers a key in the input map
		void trigger_key(Key key, KeyAction action, KeyModifiers mods);

		// triggers a button in the input map
		void trigger_mouse_click(MouseButton button, KeyAction action, KeyModifiers mods);

		// triggers a scroll in the input map
		void trigger_mouse_scroll(float dx, float dy);

		// triggers a move in the input map
		void trigger_mouse_move(float x, float y);

		// triggers a gamepad connect
		void trigger_gamepad_connect(int controller);

		// triggers a gamepad disconnect
		void trigger_gamepad_disconnect(int controller);

		// triggers a gamepad button in the input map
		void trigger_gamepad_button(int controller, GamepadButton button, KeyAction action);

		// triggers 
		void trigger_gamepad_axis(int controller, GamepadAxis axis, float value);

		// window resize
		static void resize_callback(GLFWwindow* const window, int const width, int const height);

		// keyboard key
		static void key_callback(GLFWwindow* const window, int key, int scancode, int action, int mods);

		// mouse button
		static void button_ballback(GLFWwindow* const window, int button, int action, int mods);

		// mouse scroll
		static void scroll_callback(GLFWwindow* const window, double xoffset, double yoffset);

		// cursor position
		static void cursor_callback(GLFWwindow* const window, double xpos, double ypos);

		// gamepad connect/disconnect
		//static void gamepad_callback(int id, int event);

		// error reporting
		static void error_callback(int const error, char const* description);

	public:
		friend String to_string(Window const& value);
	};
}

