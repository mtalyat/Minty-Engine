#pragma once

#include "M_Window.h"
#include "M_Renderer.h"
#include "M_SceneManager.h"
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
		SceneManager _sceneManager;

		float _deltaTime;
	public:
		Engine();

		~Engine();

		Window* get_window();

		Renderer* get_renderer();

		SceneManager* get_scene_manager();

		/// <summary>
		/// Gets the time that elapsed over the course of the last frame.
		/// </summary>
		/// <returns>The delta time.</returns>
		float get_delta_time();

		/// <summary>
		/// Runs the game.
		/// </summary>
		void run();
	private:
		// gets the current time as a time point
		time_point_t get_now() const;
	};
}
