#pragma once

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/ApplicationInfo.h"
#include "Minty/Core/Base.h"
#include "Minty/Debug/Logger.h"
#include "Minty/GUI/GUI.h"
#include "Minty/Input/Input.h"
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

	/// <summary>
	/// Holds data to build a new Application.
	/// </summary>
	struct ApplicationBuilder
	{
		ApplicationInfo info = { "Minty Application", MINTY_MAKE_VERSION(1, 0, 0) };
		String logPath = "log.txt";
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
		ApplicationMode m_mode;
		Logger* mp_logger;

		SceneManager m_sceneManager;
		std::vector<Ref<Scene>> m_workingScenes;
	public:
		Application()
			: m_initialized(false)
			, m_running(false)
			, m_minimized(false)
			, m_time()
			, m_targetFPS(0)
			, m_passFlags()
			, m_info()
			, m_mode()
			, mp_logger(nullptr)
			, m_sceneManager()
			, m_workingScenes()
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
		virtual void update(Time const& time) {}
		virtual void update_gui(Time const& time) {}

	private:
		Bool on_window_close(WindowCloseEvent& event);
		Bool on_window_resize(WindowResizeEvent& event);

#pragma endregion

#pragma region Loading

	private:
		Bool load_initial_scene();

#pragma endregion


	public:
		ApplicationInfo const& get_info() const { return m_info; }

		ApplicationMode get_mode() const { return m_mode; }

		Logger& get_logger() const { return *mp_logger; }

		SceneManager& get_scene_manager() { return m_sceneManager; }

		Time const& get_time() const { return m_time; }

		ApplicationPassFlags get_pass_flags() const { return m_passFlags; }

		Bool has_pass_flag(ApplicationPassFlags const flag) const { return (m_passFlags & flag) == flag; }

	public:
		static Application& instance() { return *sp_instance; }
	};
}