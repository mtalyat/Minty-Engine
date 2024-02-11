#pragma once

#include "M_InputMap.h"
#include "M_Window.h"
#include "M_RenderEngine.h"
#include "M_AudioEngine.h"
#include "M_SceneManager.h"
#include "M_Info.h"
#include "M_Time.h"

namespace minty
{
	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class Engine
	{
	private:
		Info _info;
		Time _time;
		InputMap _globalInput;
		Window* _window;
		RenderEngine* _renderEngine;
		AudioEngine* _audioEngine;
		SceneManager* _sceneManager;

		size_t _frameCount;
		bool _running;
		int _exitCode;
		bool _personalWindow;
	public:
		/// <summary>
		/// Creates a new Engine.
		/// </summary>
		/// <param name="appInfo">The application info.</param>
		Engine(Info const& appInfo);

		~Engine();

#pragma region Getters

		Info const& get_info() const;

		Time const& get_time() const;

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
		Window& get_window() const;

		/// <summary>
		/// Gets the RenderEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		RenderEngine& get_render_engine() const;

		/// <summary>
		/// Gets the AudioEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		AudioEngine& get_audio_engine() const;

		/// <summary>
		/// Gets the SceneManager for this Engine.
		/// </summary>
		/// <returns></returns>
		SceneManager& get_scene_manager() const;

#pragma endregion

#pragma region Exit

		/// <summary>
		/// Gets the exit code for this Engine.
		/// </summary>
		/// <returns></returns>
		int get_exit_code() const;

		/// <summary>
		/// Aborts the program with an error message.
		/// </summary>
		/// <param name="code">Any non-zero value. If zero, the message is logged and a normal exit is performed.</param>
		/// <param name="message"></param>
		void abort(int const code, String const& message);

		/// <summary>
		/// Exits the engine with the given code.
		/// </summary>
		/// <param name="code"></param>
		void exit(int const code);

#pragma endregion

#pragma region Control

		/// <summary>
		/// Initializes the engine using the given window, or creates a new one if none given.
		/// </summary>
		bool init(Window* const window = nullptr);

		/// <summary>
		/// Starts the engine, if it is not already running.
		/// </summary>
		/// <returns></returns>
		bool start();

		/// <summary>
		/// Repeats the loop as long as the engine is running.
		/// </summary>
		void run();

		/// <summary>
		/// Runs one instance of the engine loop.
		/// </summary>
		bool loop();

		/// <summary>
		/// Stops the engine, if it is running.
		/// </summary>
		void stop();

		/// <summary>
		/// Cleans up any resources from running the engine.
		/// </summary>
		void cleanup();

		/// <summary>
		/// Destroys and cleans up the engine.
		/// </summary>
		void destroy();

#pragma endregion

	private:
		/// <summary>
		/// Updates the _time object.
		/// </summary>
		void record_time();

	public:
		friend String to_string(Engine const& value);
	};
}
