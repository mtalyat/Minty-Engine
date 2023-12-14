#pragma once

#include "M_InputMap.h"
#include "M_Window.h"
#include "M_RenderEngine.h"
#include "M_AudioEngine.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_SceneManager.h"
#include "M_Info.h"
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
		InputMap _globalInput;
		Window _window;
		RenderEngine _renderEngine;
		AudioEngine _audioEngine;
		SceneManager _sceneManager;

		float _deltaTime;
	public:
		/// <summary>
		/// Creates a new Engine.
		/// </summary>
		/// <param name="appInfo">The application info.</param>
		Engine(Info const& appInfo);

		~Engine();

		/// <summary>
		/// Gets the global input map that is persistent across all Scenes and Windows.
		/// </summary>
		/// <returns></returns>
		InputMap& get_global_input_map();

		/// <summary>
		/// Gets the global input map that is persistent across all Scenes and Windows.
		/// </summary>
		/// <returns></returns>
		InputMap const& get_global_input_map() const;

		/// <summary>
		/// Gets the Window for this Engine.
		/// </summary>
		/// <returns></returns>
		Window& get_window();

		/// <summary>
		/// Gets the Window for this Engine.
		/// </summary>
		/// <returns></returns>
		Window const& get_window() const;

		/// <summary>
		/// Gets the RenderEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		RenderEngine& get_render_engine();

		/// <summary>
		/// Gets the RenderEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		RenderEngine const& get_render_engine() const;

		/// <summary>
		/// Gets the AudioEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		AudioEngine& get_audio_engine();

		/// <summary>
		/// Gets the AudioEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		AudioEngine const& get_audio_engine() const;

		/// <summary>
		/// Gets the SceneManager for this Engine.
		/// </summary>
		/// <returns></returns>
		SceneManager& get_scene_manager();

		/// <summary>
		/// Gets the SceneManager for this Engine.
		/// </summary>
		/// <returns></returns>
		SceneManager const& get_scene_manager() const;

		/// <summary>
		/// Gets the time that elapsed over the course of the last frame.
		/// </summary>
		/// <returns>The delta time.</returns>
		float get_delta_time() const;

		/// <summary>
		/// Runs the game.
		/// </summary>
		void run();
	private:
		// gets the current time as a time point
		time_point_t get_now() const;

	public:
		friend std::string to_string(Engine const& value);
	};
}
