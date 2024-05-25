#pragma once
#include "Minty/Types/M_Object.h"
#include "Minty/Layers/M_Layer.h"
#include "Minty/Types/M_Time.h"

namespace Minty
{
	class Event;
	class AssetEngine;
	class AudioEngine;
	class RenderEngine;
	class ScriptEngine;
	class DefaultLayer;
	class SceneManager;
	class WindowCloseEvent;
	class WindowResizeEvent;

	struct RuntimeBuilder;
	class Runtime;
	class Window;
	class Engine;
	class Scene;

	enum class ApplicationMode
	{
		Normal,
		Edit,
	};

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

	struct ApplicationInfo
	{
		// info
		String name = "Minty Application";
		uint32_t major = 0u;
		uint32_t minor = 0u;
		uint32_t patch = 0u;
	};

	struct ApplicationBuilder
	{
		// info
		ApplicationInfo info;

		// modular engines
		AssetEngine* assetEngine = nullptr;
		AudioEngine* audioEngine = nullptr;
		RenderEngine* renderEngine = nullptr;
		ScriptEngine* scriptEngine = nullptr;

		// config
		ApplicationMode mode = ApplicationMode::Normal;
		String currentWorkingDirectory = ".";
		ApplicationCommandLineArguments commandLineArguments;
	};

	class Application
		: public Object
	{
	private:
		ApplicationInfo _info;
		ApplicationMode _mode;

		Scope<Window> _window;
		bool _minimized;

		std::vector<Engine*> _engines;

		LayerManager _layerManager;
		DefaultLayer* _defaultLayer;

		bool _running;
		Time _time;
		TimePoint _timeStart;
		TimePoint _timeEnd;

		static Application* _instance;
	public:
		Application(ApplicationBuilder const& builder);

		virtual ~Application();

		ApplicationInfo const& get_info() const { return _info; }
		ApplicationMode const get_mode() const { return _mode; }
		Window& get_window() const { return *_window; }
		DefaultLayer& get_default_layer() const { return *_defaultLayer; }
		SceneManager& get_scene_manager() const;
		Time get_time() const { return _time; }

		void push_engine(Engine* const engine);
		void set_loaded_scene(Ref<Scene> const scene);
		void set_working_scene(Ref<Scene> const scene);

		void run();

		void on_event(Event& event);

		void push_layer(Layer* const layer);
		void push_overlay(Layer* const layer);

		void close();

		static Application& instance() { return *_instance; }

	protected:
		void load_starting_scene();

	private:
		void load_assemblies();

		void reset_time();
		void update_time();

		bool on_window_close(WindowCloseEvent& event);
		bool on_window_resize(WindowResizeEvent& event);
	};
}