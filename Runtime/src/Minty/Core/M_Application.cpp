#include "pch.h"
#include "M_Application.h"

#include "Minty/Core/M_Window.h"

#include "Minty/Events/M_ApplicationEvent.h"
#include "Minty/Events/M_KeyEvent.h"
#include "Minty/Events/M_MouseEvent.h"

#include "Minty/Layers/M_DefaultLayer.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Audio/M_AudioEngine.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Scripting/M_ScriptLink.h"

#include "Minty/Input/M_Input.h"

using namespace Minty;

#define CREATE_DEFAULT_ENGINE_IF_NEEDED(type, name) (builder.name ? builder.name : new type())

Minty::Application::Application(ApplicationBuilder const& builder)
	: _info(builder.info)
	, _mode(builder.mode)
	, _window()
	, _minimized()
	, _engines()
	, _layerManager()
	, _defaultLayer()
	, _running()
	, _time()
{
	MINTY_ASSERT(!_instance);

	_instance = this;

	// create engines
	push_engine(CREATE_DEFAULT_ENGINE_IF_NEEDED(AssetEngine, assetEngine));
	push_engine(CREATE_DEFAULT_ENGINE_IF_NEEDED(AudioEngine, audioEngine));
	push_engine(CREATE_DEFAULT_ENGINE_IF_NEEDED(RenderEngine, renderEngine));
	push_engine(CREATE_DEFAULT_ENGINE_IF_NEEDED(ScriptEngine, scriptEngine));

	// init static things
	load_assemblies();
	Input::set_input_script(ScriptEngine::instance().find_class(SCRIPT_NAMESPACE_NAME, "Input"));

	// create window
	_window = Window::create();

	// create default layer
	_defaultLayer = new DefaultLayer();
	_layerManager.push_layer(_defaultLayer);
}

#undef CREATE_DEFAULT_ENGINE_IF_NEEDED

Minty::Application::~Application()
{
	_instance = nullptr;

	delete _defaultLayer;
}

void Minty::Application::push_engine(Engine* const engine)
{
	_engines.push_back(engine);
}

void Minty::Application::set_loaded_scene(Ref<Scene> const scene)
{
	for (Engine* const engine : _engines)
	{
		engine->set_loaded_scene(scene);
	}
}

void Minty::Application::set_working_scene(Ref<Scene> const scene)
{
	for (Engine* const engine : _engines)
	{
		engine->set_working_scene(scene);
	}
}

void Minty::Application::run()
{
	MINTY_ASSERT(!_running);
	_running = true;

	reset_time();

	while (_running)
	{
		// record time
		update_time();

		_window->on_update();

		if (!_minimized)
		{
			// update all layers
			for (Layer* layer : _layerManager)
			{
				layer->on_update(_time);
			}
		}
	}
}

void Minty::Application::on_event(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_close));
	dispatcher.dispatch<WindowResizeEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_resize));

	for (auto it = _layerManager.rbegin(); !event.handled && it != _layerManager.rend(); it++)
	{
		(*it)->on_event(event);
	}
}

void Minty::Application::push_layer(Layer* const layer)
{
	_layerManager.push_layer(layer);
}

void Minty::Application::push_overlay(Layer* const layer)
{
	_layerManager.push_overlay(layer);
}

void Minty::Application::close()
{
	_running = false;
}

static void load_script_assemblies(std::vector<Path> const& paths)
{
	ScriptEngine& scripts = ScriptEngine::instance();
	for (Path const& assemblyPath : paths)
	{
		scripts.load_assembly(assemblyPath);
	}
}

void Minty::Application::load_assemblies()
{
	AssetEngine& assets = AssetEngine::instance();
	Path applicationDataPath = String("game").append(EXTENSION_APPLICATION_DATA);

	if (assets.exists(applicationDataPath))
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
			load_script_assemblies(paths);
		}
		else
		{
			load_script_assemblies({});
		}

		// connect C++ to C#
		ScriptLink::link();

		if (Node const* scenes = node.find("scenes"))
		{
			SceneManager& sceneManager = _defaultLayer->get_scene_manager();
			Scene& scene = sceneManager.create_scene(scenes->get_children().front().get_data());
			sceneManager.load_scene(scene.get_id());
		}
	}
	else
	{
		MINTY_WARN("No application data found.");
	}
}

void Minty::Application::reset_time()
{
	TimePoint now = Time::now();

	_timeStart = now;
	_timeEnd = now;

	_time.total = 0.0f;
	_time.elapsed = 0.0f;
}

void Minty::Application::update_time()
{
	// get the current time
	TimePoint now = Time::now();

	// calculate the times in seconds
	_time.total = Time::calculate_duration_seconds(_timeStart, now);
	_time.elapsed = Time::calculate_duration_seconds(_timeEnd, now);

	_timeEnd = now;
}

bool Minty::Application::on_window_close(WindowCloseEvent& event)
{
	close();
	return true;
}

bool Minty::Application::on_window_resize(WindowResizeEvent& event)
{
	if (event.get_width() == 0 || event.get_height() == 0)
	{
		_minimized = true;
		return false;
	}

	_minimized = false;
	// TODO: SEND EVENT TO RENDERER

	return false;
}

// LINKING
/*
MINTY_LOG("Linking the Runtime:");

	
*/

// REGISTER STUFF
/*

*/