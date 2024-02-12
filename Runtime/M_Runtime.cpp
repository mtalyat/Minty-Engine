#include "pch.h"
#include "M_Runtime.h"

#include "M_RendererBuilder.h"
#include "M_Info.h"
#include "M_Console.h"

#include "M_RenderEngine.h"
#include "M_AudioEngine.h"

#include "M_SystemRegistry.h"
#include "M_AnimationSystem.h"
#include "M_AudioSystem.h"
#include "M_RenderSystem.h"
#include "M_UISystem.h"

#include "M_EntityRegistry.h"
#include "M_AnimatorComponent.h"
#include "M_AudioListenerComponent.h"
#include "M_AudioSourceComponent.h"
#include "M_CameraComponent.h"
#include "M_DirtyComponent.h"
#include "M_MeshComponent.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_RenderableComponent.h"
#include "M_SpriteComponent.h"
#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"

#include "M_GLFW.h"
#include <iostream>

using namespace minty;
using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Runtime::Runtime(Info const& info)
	: _info(info)
	, _globalInput()
	, _window()
	, _engines()
	, _enginesLookup()
	, _sceneManager()
	, _initialized()
	, _frameCount()
	, _running()
	, _exitCode()
	, _personalWindow()
{}

Runtime::~Runtime()
{
	stop();
	destroy();
}

Info const& minty::Runtime::get_info() const
{
	return _info;
}

Time const& minty::Runtime::get_time() const
{
	return _time;
}

bool minty::Runtime::is_running() const
{
	return _running;
}

InputMap& minty::Runtime::get_global_input_map()
{
	return _globalInput;
}

InputMap const& minty::Runtime::get_global_input_map() const
{
	return _globalInput;
}

Window& minty::Runtime::get_window() const
{
	MINTY_ASSERT(_initialized, "Runtime::get_window(): Runtime is not initialized.");

	return *_window;
}

RenderEngine& minty::Runtime::get_render_engine() const
{
	return *static_cast<RenderEngine*>(_engines[RENDER_ENGINE_INDEX]);
}

AudioEngine& minty::Runtime::get_audio_engine() const
{
	return *static_cast<AudioEngine*>(_engines[AUDIO_ENGINE_INDEX]);
}

SceneManager& minty::Runtime::get_scene_manager() const
{
	MINTY_ASSERT(_initialized, "Runtime::get_scene_manager(): Runtime is not initialized.");

	return *_sceneManager;
}

int minty::Runtime::get_exit_code() const
{
	return _exitCode;
}

void minty::Runtime::abort(int const code, String const& message)
{
	if (code)
	{
		Console::error(message);
	}

	exit(code);
}

void minty::Runtime::exit(int const code)
{
	_exitCode = code;
	_running = false;
}

bool minty::Runtime::init(RuntimeBuilder const& builder)
{
	if (_initialized) return true;

	_initialized = true;

	// create necessary components
	_personalWindow = builder.window == nullptr;
	_window = _personalWindow ? new Window(_info.get_application_name(), WIDTH, HEIGHT, &_globalInput) : builder.window;
	_sceneManager = new SceneManager(*this);

	set_engine<RenderEngine>(builder.renderEngine ? builder.renderEngine : new RenderEngine());
	set_engine<AudioEngine>(builder.audioEngine ? builder.audioEngine : new AudioEngine());

	// perform static operations that happen once
	static bool registered = false;

	if (!registered)
	{
		// systems
		SystemRegistry::register_system<AnimationSystem>("Animation");
		SystemRegistry::register_system<AudioSystem>("Audio");
		SystemRegistry::register_system<RenderSystem>("Render");
		SystemRegistry::register_system<UISystem>("UI");

		// components
		EntityRegistry::register_component<AnimatorComponent>("Animator");
		EntityRegistry::register_component<AudioListenerComponent>("AudioListener");
		EntityRegistry::register_component<AudioSourceComponent>("AudioSource");
		EntityRegistry::register_component<CameraComponent>("Camera");
		EntityRegistry::register_component<DirtyComponent>("Dirty");
		EntityRegistry::register_component<MeshComponent>("Mesh");
		EntityRegistry::register_component<NameComponent>("Name");
		EntityRegistry::register_component<RelationshipComponent>("Relationship");
		EntityRegistry::register_component<RenderableComponent>("Renderable");
		EntityRegistry::register_component<SpriteComponent>("Sprite");
		EntityRegistry::register_component<TransformComponent>("Transform");
		EntityRegistry::register_component<UITransformComponent>("UITransform");

		registered = true;
	}

	return true;
}

bool minty::Runtime::start()
{
	if (_running) return true; // already started

	// if no scenes at all, abort
	if (!_sceneManager->size())
	{
		Console::error("Aborting game. No Scenes loaded.");
		return false;
	}

	// if no scene loaded, just load the first scene
	if (!_sceneManager->get_loaded_scene())
	{
		_sceneManager->load_scene(0);
	}

	// start the scene(s)
	_sceneManager->load();

	// get the recorded time
	_time.start = Time::now();
	_time.end = _time.start;
	record_time();

	_running = true;

	return true;
}

void Runtime::run()
{
	while (_running && loop()) {}
}

bool minty::Runtime::loop()
{
	// stop running if window is closed
	if (!_window->is_open())
	{
		stop();
		return false;
	}

	// run window events
	glfwPollEvents();

	// update scene(s)
	_sceneManager->update();

	// cleanup scene
	_sceneManager->finalize();

	// render to the screen
	get_render_engine().render_frame();

	// move to next frame
	_frameCount++;

	// update time and elapsed time
	record_time();

	return true;
}

void minty::Runtime::stop()
{
	if (_running)
	{
		exit(0);
	}
}

void minty::Runtime::cleanup()
{
	// wait for Vulkan to sync so no thread racing
	get_render_engine().sync();

	// clean up scenes
	_sceneManager->unload();
	_sceneManager->destroy();
}

void minty::Runtime::destroy()
{
	if (!_initialized) return;

	_initialized = false;

	if (_sceneManager)
	{
		delete _sceneManager;
		_sceneManager = nullptr;
	}

	for (auto const& engine : _engines)
	{
		delete engine;
	}
	_engines.clear();
	_enginesLookup.clear();

	if (_window && _personalWindow)
	{
		delete _window;
		_window = nullptr;
	}
}

void minty::Runtime::record_time()
{
	// get the current time
	TimePoint now = Time::now();

	// calculate the times in seconds
	_time.time = Time::calculate_duration_seconds(_time.start, now);
	_time.elapsed = Time::calculate_duration_seconds(_time.end, now);

	_time.end = now;
}

String minty::to_string(Runtime const& value)
{
	return std::format("Runtime()");
}
