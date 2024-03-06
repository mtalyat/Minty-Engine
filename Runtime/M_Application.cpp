#include "pch.h"
#include "M_Application.h"

#include "M_Runtime.h"
#include "M_ScriptEngine.h"
#include "M_RenderEngine.h"

using namespace minty;

minty::Application::Application(Info const& info, Path const& path)
	: _info(info)
	, _path(path)
	, _window()
	, _runtime()
{
	std::filesystem::current_path(path);
}

minty::Application::~Application()
{}

void minty::Application::create()
{
	if (_runtime) return;

	_window = create_window();
	_runtime = create_runtime();
}

void minty::Application::init(RuntimeBuilder* builder)
{
	if (!_runtime) return;

	_runtime->init(builder);

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
}

void minty::Application::destroy()
{
	if (!_runtime) return;

	// stop if needed
	if (_runtime->is_running())
	{
		_runtime->stop();
		_runtime->cleanup();
	}

	_runtime->destroy();
	delete _runtime;
}

int minty::Application::run()
{
	create();
	init();
	start();
	loop();
	stop();
	destroy();

	return get_exit_code();
}

int minty::Application::get_exit_code() const
{
	return _runtime->get_exit_code();
}

Window* minty::Application::create_window()
{
	return new Window("", 1280, 720, "Icon.png");
}

Runtime* minty::Application::create_runtime()
{
	return new Runtime(_info);
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

Window& minty::Application::get_window() const
{
	return *_window;
}

Runtime& minty::Application::get_runtime() const
{
	return *_runtime;
}

Path const& minty::Application::get_path() const
{
	return _path;
}

Info const& minty::Application::get_info() const
{
	return _info;
}
