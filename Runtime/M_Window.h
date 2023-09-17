#pragma once

#include "M_Object.h"
#include "M_Event.h"
#include "M_InputMap.h"
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
		GLFWwindow* _window;
		bool _resized;
		InputMap const* _activeInputMap;
		float _lastMouseX, _lastMouseY;
		bool _mouseOutOfBounds;

	public:
		Window(std::string const& title, int const width, int const height);

		~Window();

		/// <summary>
		/// Checks if the Window has resized since the last time this method was called.
		/// </summary>
		/// <returns>True if the Window has been resized.</returns>
		bool isResized();

		/// <summary>
		/// Checks if the Window is currently open
		/// </summary>
		/// <returns>True if the Window is not closed.</returns>
		bool isOpen() const;

		/// <summary>
		/// Gets the size of the Window, in pixels.
		/// </summary>
		/// <param name="width">The width in pixels.</param>
		/// <param name="height">The height in pixels.</param>
		void getFramebufferSize(int* const width, int* const height) const;

		/// <summary>
		/// Gets the raw GLFWwindow component.
		/// </summary>
		/// <returns>The raw window.</returns>
		GLFWwindow* getRaw() const;

		void setInput(InputMap const* const inputMap);
	private:
		void triggerKey(Key const key, KeyAction const action, KeyModifiers const mods);

		void triggerButton(MouseButton const button, KeyAction const action, KeyModifiers const mods);

		void triggerScroll(float dx, float dy);

		void triggerCursor(float x, float y);

		// window resize
		static void resizeCallback(GLFWwindow* const window, int const width, int const height);

		// keyboard key
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		// mouse button
		static void buttonCallback(GLFWwindow* window, int button, int action, int mods);

		// mouse scroll
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		// cursor position
		static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	};
}

