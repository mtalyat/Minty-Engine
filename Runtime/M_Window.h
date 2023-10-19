#pragma once

#include "M_Object.h"
#include "M_Event.h"
#include "M_InputMap.h"
#include "M_CursorMode.h"
#include <map>

struct GLFWwindow;

namespace minty
{
	/// <summary>
	/// A window on the screen.
	/// </summary>
	class Window
		: public Object
	{
	private:
		static int _windowCount;

		std::string _title;
		GLFWwindow* _window;
		bool _resized;
		InputMap const* _activeInputMap;
		float _lastMouseX, _lastMouseY;
		bool _mouseOutOfBounds;

	public:
		/// <summary>
		/// Creates a new Window.
		/// </summary>
		/// <param name="title">The title to be displayed on the Window.</param>
		/// <param name="width">The width of the Window in pixels.</param>
		/// <param name="height">The height of the Window in pixels.</param>
		Window(std::string const& title, int const width, int const height);

		~Window();

		/// <summary>
		/// Sets the title text of this Window.
		/// </summary>
		void set_title(std::string const& title);

		/// <summary>
		/// Gets the title text of this Window.
		/// </summary>
		std::string get_title() const;

		/// <summary>
		/// Sets the cursor mode.
		/// </summary>
		/// <param name="mode"></param>
		void set_cursor_mode(CursorMode const mode);

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
		/// Gets the size of the Window, in pixels.
		/// </summary>
		/// <param name="width">The width in pixels.</param>
		/// <param name="height">The height in pixels.</param>
		void get_framebuffer_size(int* const width, int* const height) const;

		/// <summary>
		/// Gets the raw GLFWwindow component.
		/// </summary>
		/// <returns>The raw window.</returns>
		GLFWwindow* get_raw() const;

		/// <summary>
		/// Sets the currently active InputMap for this Window.
		/// </summary>
		/// <param name="inputMap"></param>
		void set_input(InputMap const* const inputMap);

		/// <summary>
		/// Gets the currently active InputMap for this Window.
		/// </summary>
		/// <returns></returns>
		InputMap const* get_input() const;
	private:
		// triggers a key in the input map
		void trigger_key(Key const key, KeyAction const action, KeyModifiers const mods);

		// triggers a button in the input map
		void trigger_button(MouseButton const button, KeyAction const action, KeyModifiers const mods);

		// triggers a scroll in the input map
		void trigger_scroll(float dx, float dy);

		// triggers a move in the input map
		void trigger_cursor(float x, float y);

		// window resize
		static void resize_callback(GLFWwindow* const window, int const width, int const height);

		// keyboard key
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		// mouse button
		static void button_ballback(GLFWwindow* window, int button, int action, int mods);

		// mouse scroll
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		// cursor position
		static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

	public:
		friend std::string to_string(Window const& value);
	};
}

