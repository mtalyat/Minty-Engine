#include "pch.h"
#include "M_Application.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_ScriptEngine.h"
#include "M_RenderEngine.h"
#include "M_SceneManager.h"
#include "M_Scene.h"

using namespace minty;

minty::Application::Application(RunMode const mode)
	: _info("Minty", 0, 0, 0)
	, _mode(mode)
	, _window()
	, _runtime()
{}

minty::Application::~Application()
{}

void minty::Application::create()
{
	if (_runtime) return;

	_window = create_window();
	MINTY_ASSERT(_window != nullptr);
	_runtime = create_runtime();
	MINTY_ASSERT(_runtime != nullptr);
}

void minty::Application::init(RuntimeBuilder* builder)
{
	if (!_runtime) return;

	_runtime->init(*_window, builder);

	// init the render engine
	RenderEngineBuilder renderEngineBuilder{
		.info = &_info,
		.window = _window
	};
	_runtime->get_render_engine().init(renderEngineBuilder);
}

void minty::Application::start()
{
	if (!_runtime) return;

	// load the first scene
	// TODO: make this... not so janky
	AssetEngine& assets = _runtime->get_asset_engine();
	Path applicationDataPath = String("game").append(EXTENSION_APPLICATION_DATA);

	if (std::filesystem::exists(applicationDataPath))
	{
		Node node = assets.read_file_node(applicationDataPath);

		// load assemblies
		if (Node const* assemblies = node.find("assemblies"))
		{
			// get paths to assemblies
			std::vector<Path> paths;
			paths.reserve(assemblies->get_children().size());

			for (auto const& assembly : assemblies->get_children())
			{
				paths.push_back(assembly.to_string());
			}

			// load these
			load_assemblies(paths);
		}
		else
		{
			load_assemblies({});
		}

		if (Node const* scenes = node.find("scenes"))
		{
			SceneManager& sceneManager = _runtime->get_scene_manager();
			Scene& scene = sceneManager.create_scene(scenes->get_children().front().get_data());
			sceneManager.load_scene(scene.get_id());
		}
	}
	else
	{
		MINTY_WARN("No application data found.");
	}

	_runtime->start();
}

void minty::Application::loop()
{
	if (!_runtime) return;

	_runtime->run();
	_runtime->cleanup();
}

void minty::Application::stop()
{
	if (!_runtime) return;

	_runtime->stop();
	_runtime->cleanup();
}

void minty::Application::destroy()
{
	if (!_runtime) return;

	_window->close();
	MINTY_DELETE(_window);

	_runtime->destroy();
	MINTY_DELETE(_runtime);
}

int minty::Application::run()
{
	create();
	init();
	start();
	loop();
	stop();

	int code = _runtime->get_exit_code();

	destroy();

	return code;
}

Window* minty::Application::create_window()
{
	return new Window("", 1280, 720);
}

Runtime* minty::Application::create_runtime()
{
	return new Runtime(_info, _mode);
}

void minty::Application::load_assemblies(std::vector<Path> const& paths)
{
	ScriptEngine& scripts = _runtime->get_script_engine();
	for (Path const& assemblyPath : paths)
	{
		scripts.load_assembly(assemblyPath);
	}
	_runtime->link();
}

RunMode minty::Application::get_mode() const
{
	return _mode;
}

Window& minty::Application::get_window() const
{
	return *_window;
}

Runtime& minty::Application::get_runtime() const
{
	return *_runtime;
}

Info const& minty::Application::get_info() const
{
	return _info;
}
