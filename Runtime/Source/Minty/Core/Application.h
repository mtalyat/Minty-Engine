#pragma once

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/ApplicationInfo.h"
#include "Minty/Core/Base.h"
#include "Minty/Debug/Logger.h"
#include "Minty/GUI/GUI.h"
#include "Minty/Input/Input.h"
#include "Minty/Layer/LayerManager.h"
#include "Minty/Render/Renderer.h"
#include "Minty/Scene/SceneManager.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/Window/WindowManager.h"

namespace Minty
{
	/// <summary>
	/// Holds the command line arguments.
	/// </summary>
	struct ApplicationCommandLineArguments
	{
		int argc = 0;
		char** argv = nullptr;

		char const* operator[](int index) const
		{
			MINTY_ASSERT(index >= 0);
			MINTY_ASSERT(index < argc);

			return argv[index];
		}
	};

	/// <summary>
	/// Determines how the resources (such as scripts) are loaded into the Application.
	/// </summary>
	enum class ApplicationMode
	{
		Normal,
		Edit,
	};

	enum class ApplicationPassFlags
	{
		None = 0,
		Scene = 1,
		Gui = 2,
		All = 3,
	};
	MINTY_ENUM_FLAGS_OPERATORS(ApplicationPassFlags);

	struct ApplicationData
	{
		// first scene to load
		Path initialScene = "";
		// additional assemblies to load (other than MintyEngine.dll)
		std::vector<Path> assemblies = {};
		// additional wraps to load (other than Game.wrap)
		std::vector<Path> wraps = {};
	};

	/// <summary>
	/// Holds data to build a new Application.
	/// </summary>
	struct ApplicationBuilder
	{
		ApplicationInfo info = { "Minty Application", MINTY_MAKE_VERSION(1, 0, 0) };
		ApplicationData data = {};
		ApplicationMode mode = ApplicationMode::Normal;
		UInt targetFPS = 120;
		ApplicationPassFlags passes = ApplicationPassFlags::All;

		AssetManagerBuilder assetManagerBuilder{};
		ScriptEngineBuilder scriptEngineBuilder{};
		InputBuilder inputBuilder{};
		WindowManagerBuilder windowManagerBuilder{};
		RendererBuilder rendererBuilder{};
		GUIBuilder guiBuilder{};
	};

	/// <summary>
	/// A Minty Engine application.
	/// </summary>
	class Application
	{
	private:
		static Application* sp_instance;

		Bool m_initialized;
		Bool m_running;
		Bool m_minimized;

		Time m_time;

		UInt m_targetFPS;

		ApplicationPassFlags m_passFlags;

		ApplicationInfo m_info;
		ApplicationData m_data;
		ApplicationMode m_mode;

		SceneManager m_sceneManager;
		std::vector<Ref<Scene>> m_workingScenes;

		LayerManager m_layerManager;
	public:
		Application()
			: m_initialized(false)
			, m_running(false)
			, m_minimized(false)
			, m_time()
			, m_targetFPS(0)
			, m_passFlags()
			, m_info()
			, m_data()
			, m_mode()
			, m_sceneManager()
			, m_workingScenes()
			, m_layerManager()
		{}

		~Application()
		{}

		void initialize(ApplicationBuilder const& builder);

		void shutdown();

		Int run();

		void close();

#pragma region Events

	public:
		void on_event(Event& event);

	protected:
		// runs every update loop
		virtual Int update(Time const& time) { return 0; }
		virtual Int update_gui(Time const& time) { return 0; }

	private:
		Bool on_window_close(WindowCloseEvent& event);
		Bool on_window_resize(WindowResizeEvent& event);

#pragma endregion

#pragma region Loading

	protected:
		// removes all config settings
		Bool clear_config();

		// loads a config file from the given path
		Bool load_config(Path const& path);

	private:
		// loads the default application data
		Bool load_data();

		// loads the game.wrap file: important application files
		Bool load_game_wrap();

		// loads the game.appdata file: initial data (wraps, dlls, etc.)
		Bool load_game_appdata();

		// loads the config.appdata file: initial config (engine data, layers, etc.)
		Bool load_game_config();

		// loads the initial scene from the app data, if there is one
		Bool load_initial_scene();

#pragma endregion


	public:
		ApplicationInfo const& get_info() const { return m_info; }

		ApplicationMode get_mode() const { return m_mode; }

		SceneManager& get_scene_manager() { return m_sceneManager; }

		LayerManager& get_layer_manager() { return m_layerManager; }

		Time const& get_time() const { return m_time; }

		ApplicationPassFlags get_pass_flags() const { return m_passFlags; }

		Bool has_pass_flag(ApplicationPassFlags const flag) const { return (m_passFlags & flag) == flag; }

	public:
		static Application& instance() { return *sp_instance; }
	};
}