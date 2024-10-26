#pragma once
#include "Minty/Core/ApplicationInfo.h"
#include "Minty/Core/Base.h"
#include "Minty/Core/WindowManager.h"
#include "Minty/Debug/Logger.h"
#include "Minty/Scene/SceneManager.h"

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

	/// <summary>
	/// Holds data to build a new Application.
	/// </summary>
	struct ApplicationBuilder
	{
		ApplicationInfo info = { "Minty Application", MINTY_MAKE_VERSION(1, 0, 0) };
		String logPath = "log.txt";
		ApplicationMode mode = ApplicationMode::Normal;
		UInt targetFPS = 120;
	};

	/// <summary>
	/// A Minty Engine application.
	/// </summary>
	class Application
	{
	private:
		static Application* sp_instance;

		Bool m_running;
		Bool m_minimized;

		Time m_time;

		UInt m_targetFPS;

		ApplicationInfo m_info;
		ApplicationMode m_mode;
		Logger m_logger;

		SceneManager m_sceneManager;
		std::vector<Ref<Scene>> m_workingScenes;
	public:
		Application(const ApplicationBuilder& builder);

		~Application();

		Int run();

		void close();

#pragma region Events

	public:
		void on_event(Event& event);

	private:
		Bool on_window_close(WindowCloseEvent& event);
		Bool on_window_resize(WindowResizeEvent& event);

#pragma endregion

	public:
		const ApplicationInfo& get_info() const { return m_info; }

		ApplicationMode get_mode() const { return m_mode; }

		Logger& get_logger() { return m_logger; }

		SceneManager& get_scene_manager() { return m_sceneManager; }

		Time const& get_time() const { return m_time; }

	public:
		static Application& instance() { return *sp_instance; }
	};
}