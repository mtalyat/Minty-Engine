#pragma once

#include "M_Window.h"
#include "M_Renderer.h"
#include <chrono>

typedef std::chrono::steady_clock::time_point time_point_t;

namespace minty
{
	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class Engine
	{
	private:
		Window _window;
		Renderer _renderer;


	public:
		Engine();

		~Engine();

		Window& get_window();

		Renderer& get_renderer();

		/// <summary>
		/// Runs the game.
		/// </summary>
		void run();
	private:
		// gets the current time as a time point
		time_point_t get_now() const;
	};
}
