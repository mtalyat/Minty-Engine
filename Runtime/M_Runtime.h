#pragma once

#include "M_RuntimeBuilder.h"
#include "M_InputMap.h"
#include "M_Window.h"
#include "M_SceneManager.h"
#include "M_Info.h"
#include "M_Time.h"
#include "M_Types.h"
#include <unordered_map>

namespace minty
{
	class Engine;
	class RenderEngine;
	class AudioEngine;

	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class Runtime
	{
	private:
		constexpr static size_t RENDER_ENGINE_INDEX = 0;
		constexpr static size_t AUDIO_ENGINE_INDEX = 0;

	private:
		Info _info;
		Time _time;
		InputMap _globalInput;
		Window* _window;
		std::vector<Engine*> _engines;
		std::unordered_map<String, size_t> _enginesLookup;
		SceneManager* _sceneManager;

		bool _initialized;
		size_t _frameCount;
		bool _running;
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
		bool init(RuntimeBuilder const& builder);

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
		friend String to_string(Runtime const& value);
	};
	
	template<typename T>
	T* Runtime::get_engine() const
	{
		MINTY_ASSERT(_initialized, "Runtime::get_engine(): Runtime is not initialized.");

		auto found = _enginesLookup.find(typeid(T).name);

		if (found != _enginesLookup.end())
		{
			return static_cast<T*>(_engines.at(found->second));
		}

		// not found
		return nullptr;
	}
	template<typename T>
	void Runtime::set_engine(T* const engine)
	{
		MINTY_ASSERT(_initialized, "Runtime::set_engine(): Runtime is not initialized.");
		bool isType = is_type<T, Engine>();
		MINTY_ASSERT(isType, "The type given to Runtime::set_engine() must derive from Engine.");

		// if engine already exists, dispose of it and replace it
		String name = typeid(T).name();
		auto found = _enginesLookup.find(name);

		if (found != _enginesLookup.end())
		{
			// destroy old engine
			delete _engines.at(found->second);

			// set new engine
			_engines[found->second] = engine;
		}
		else
		{
			// add new engine to the back
			_enginesLookup.emplace(name, _engines.size());
			_engines.push_back(engine);
		}
	}
}
