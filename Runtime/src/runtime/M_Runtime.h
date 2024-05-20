#pragma once

#include "runtime/M_RunMode.h"
#include "rendering/M_Window.h"
#include "runtime/M_Info.h"
#include "types/M_Time.h"
#include "runtime/M_Engine.h"
#include "types/M_Types.h"
#include "types/M_TypeRegister.h"
#include "systems/M_SystemRegistry.h"
#include "entities/M_EntityRegistry.h"
#include "scripting/M_ScriptEngine.h"
#include <unordered_map>

namespace minty
{
	class Engine;
	class SceneManager;
	class Assembly;

	class Window;
	class AssetEngine;
	class RenderEngine;
	class AudioEngine;
	class ScriptEngine;

	struct RuntimeBuilder
	{
		AssetEngine* assetEngine = nullptr;

		RenderEngine* renderEngine = nullptr;

		AudioEngine* audioEngine = nullptr;

		ScriptEngine* scriptEngine = nullptr;
	};

	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class Runtime
	{
		friend class SceneManager;

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
		constexpr static size_t ASSET_ENGINE_INDEX = 0;
		constexpr static size_t RENDER_ENGINE_INDEX = 1;
		constexpr static size_t AUDIO_ENGINE_INDEX = 2;
		constexpr static size_t SCRIPT_ENGINE_INDEX = 3;

	private:
		RunMode _mode;
		State _state;
		Info _info;
		Time _time;
		Window* _window;
		SceneManager* _sceneManager;
		TypeRegister<Engine> _engines;

		size_t _frameCount;
		int _exitCode;
	public:
		/// <summary>
		/// Creates a new Engine.
		/// </summary>
		/// <param name="appInfo">The application info.</param>
		Runtime(Info const& info, RunMode const mode = RunMode::Normal);

		~Runtime();

#pragma region Getters

	public:
		RunMode get_mode() const;

		Info const& get_info() const;

		Time const& get_time() const;

		bool is_running() const;

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

	public:
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
		/// Gets the AssetEngine for this Engine.
		/// </summary>
		/// <returns></returns>
		AssetEngine& get_asset_engine() const;

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

	private:
		void set_loaded_scene(Scene* const scene) const;

		void set_working_scene(Scene* const scene) const;

#pragma endregion

#pragma region Exit

	public:
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

	public:
		/// <summary>
		/// Initializes the engine using the given window, or creates a new one if none given.
		/// </summary>
		bool init(Window& window, RuntimeBuilder const* builder = nullptr);

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

#pragma region Linking

	public:
		template<class T>
		static void register_system(String const& name);

		template<class T>
		static void register_component(String const& namespaceName, String const& className, bool const link = true);

		static void register_script(String const& name);

		/// <summary>
		/// Links the C++ and C# together.
		/// </summary>
		void link();

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
		MINTY_ASSERT_MESSAGE(_state >= State::Initialized, "Runtime is not initialized.");

		return _engines.get<T>();
	}

	template<typename T>
	void Runtime::set_engine(T* const engine)
	{
		MINTY_ASSERT_MESSAGE(_state >= State::Initialized, "Runtime is not initialized.");

		// if engine already exists, dispose of it and replace it
		T* other = _engines.get<T>();
		if (other)
		{
			delete other;
		}

		// set engine
		_engines.emplace<T>(engine);
	}

	template<class T>
	void Runtime::register_system(String const& name)
	{
		SystemRegistry::register_system<T>(name);
	}

	template<class T>
	void Runtime::register_component(String const& namespaceName, String const& className, bool const link)
	{
		EntityRegistry::register_component<T>(className);
		if (link)
		{
			ScriptEngine::link_script(ScriptEngine::get_full_name(namespaceName, className));
		}
	}
}
