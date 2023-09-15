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

		void getFramebufferSize(int& width, int& height) const;

		operator GLFWwindow* () const { return _window; }

	private:
		static void resizeCallback(GLFWwindow* const window, int const width, int const height);
	};
}

