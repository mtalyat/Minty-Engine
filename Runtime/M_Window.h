#pragma once

#include <string>

struct GLFWwindow;

namespace minty
{
	/// <summary>
	/// A window on the screen.
	/// </summary>
	class Window
	{
	private:
		GLFWwindow* _window;
		bool _resized;

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

	private:
		static void resizeCallback(GLFWwindow* const window, int const width, int const height);
	};
}

