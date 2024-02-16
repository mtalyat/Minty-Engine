#pragma once

#include "M_RuntimeBuilder.h"
#include "M_Window.h"
#include "M_SceneManager.h"
#include "M_Info.h"
#include "M_Time.h"
#include "M_Engine.h"
#include "M_Types.h"
#include "M_TypeRegister.h"
#include "M_AssemblyType.h"
#include <unordered_map>

namespace minty
{
	class Engine;
	class InputMap;
	class RenderEngine;
	class AudioEngine;
	class ScriptEngine;
	class Assembly;
}

namespace minty
{
	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class Runtime
	{
	private:
		enum class State
		{
			/// <summary>
			/// The Runtime has not been initialized.
			/// </summary>
			Uninitialized,

			/// <summary>
			/// The Runtime has been initialized, but not started.
			/// </summary>
			Initialized,

			/// <summary>
			/// The Runtime is running.
			/// </summary>
			Running,

			/// <summary>
			/// The Runtime has stopped running.
			/// </summary>
			Stopped = Initialized,

			/// <summary>
			/// The Runtime has been destroyed.
			/// </summary>
			Destroyed = Uninitialized,
		};

	private:
		constexpr static size_t RENDER_ENGINE_INDEX = 0;
		constexpr static size_t AUDIO_ENGINE_INDEX = 1;
		constexpr static size_t SCRIPT_ENGINE_INDEX = 2;

	private:
		State _state;
		Info _info;
		Time _time;
		InputMap* _globalInput;
		Window* _window;
		SceneManager* _sceneManager;
		TypeRegister<Engine> _engines;

		size_t _frameCount;
		int _exitCode;
		bool _personalWindow;
	public:
		/// <summary>
		/// Creates a new Engine.
		/// </summary>
		/// <param name="appInfo">The application info.</param>
		Runtime(Info const& info);

		~Runtime();

#pragma region Getters

		Info const& get_info() const;

		Time const& get_time() const;

		bool is_running() const;

		/// <summary>
		/// Gets the global input map that is persistent across all Scenes and Windows.
		/// </summary>
		/// <returns></returns>
		InputMap& get_global_input_map() const;

		/// <summary>
		/// Gets the Window for this Engine.
		/// </summary>
		/// <returns></returns>
		Window& get_window() const;

		/// <summary>
		/// Gets the SceneManager for this Engine.
		/// </summary>
		/// <returns></returns>
		SceneManager& get_scene_manager() const;

#pragma endregion

#pragma region Engines

		/// <summary>
		/// Gets the Engine of the specific type.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		T* get_engine() const;

		/// <summary>
		/// Sets the Engine of the specific type.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="engine"></param>
		template<typename T>
		void set_engine(T* const engine);

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
		/// Gets the ScriptEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		ScriptEngine& get_script_engine() const;

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
		bool init(RuntimeBuilder const* builder = nullptr);

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

#pragma region Assemblies

		public:


#pragma endregion


	private:
		/// <summary>
		/// Updates the _time object.
		/// </summary>
		void record_time();

	public:
		friend String to_string(Runtime const& value);
	};
	
	template<typename T>
	T* Runtime::get_engine() const
	{
		MINTY_ASSERT(_state >= State::Initialized, "Runtime::get_engine(): Runtime is not initialized.");

		return _engines.get<T>();
	}

	template<typename T>
	void Runtime::set_engine(T* const engine)
	{
		MINTY_ASSERT(_state >= State::Initialized, "Runtime::set_engine(): Runtime is not initialized.");

		// set reference to self
		Engine* e = static_cast<Engine*>(engine);
		e->set_runtime(*this);

		// if engine already exists, dispose of it and replace it
		T* other = _engines.get<T>();
		if (other)
		{
			delete other;
		}

		// set engine
		_engines.emplace<T>(engine);
	}
}
