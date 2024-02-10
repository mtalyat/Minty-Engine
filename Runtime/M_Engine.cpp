#include "pch.h"
#include "M_Engine.h"

#include "M_RendererBuilder.h"
#include "M_Info.h"
#include "M_Console.h"

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

#include "glfw.hpp"
#include <iostream>

using namespace minty;
using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Engine::Engine(Info const& info)
	: _info(info)
	, _globalInput()
	, _window()
	, _renderEngine()
	, _audioEngine()
	, _sceneManager()
	, _frameCount()
	, _running()
	, _exitCode()
{}

Engine::~Engine()
{
	stop();
	destroy();
}

Info const& minty::Engine::get_info() const
{
	return _info;
}

Time const& minty::Engine::get_time() const
{
	return _time;
}

InputMap& minty::Engine::get_global_input_map()
{
	return _globalInput;
}

InputMap const& minty::Engine::get_global_input_map() const
{
	return _globalInput;
}

Window& minty::Engine::get_window() const
{
	MINTY_ASSERT(_renderEngine != nullptr, "Engine::get_window(): Engine is not initialized.");

	return *_window;
}

RenderEngine& minty::Engine::get_render_engine() const
{
	MINTY_ASSERT(_renderEngine != nullptr, "Engine::get_render_engine(): Engine is not initialized.");

	return *_renderEngine;
}

AudioEngine& minty::Engine::get_audio_engine() const
{
	MINTY_ASSERT(_renderEngine != nullptr, "Engine::get_audio_engine(): Engine is not initialized.");

	return *_audioEngine;
}

SceneManager& minty::Engine::get_scene_manager() const
{
	MINTY_ASSERT(_renderEngine != nullptr, "Engine::get_scene_manager(): Engine is not initialized.");

	return *_sceneManager;
}

int minty::Engine::get_exit_code() const
{
	return _exitCode;
}

void minty::Engine::abort(int const code, String const& message)
{
	if (code)
	{
		Console::error(message);
	}

	exit(code);
}

void minty::Engine::exit(int const code)
{
	_exitCode = code;
	_running = false;
}

bool minty::Engine::init(Window* const window)
{
	// destroy if needed
	destroy();

	// create necessary components
	_window = window != nullptr ? window : new Window(_info.get_application_name(), WIDTH, HEIGHT, &_globalInput);
	_renderEngine = new RenderEngine(_window);
	_audioEngine = new AudioEngine();
	_sceneManager = new SceneManager(*this);

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

bool minty::Engine::start()
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

void Engine::run()
{
	while (_running && loop()) {}
}

bool minty::Engine::loop()
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
	_renderEngine->render_frame();

	// move to next frame
	_frameCount++;

	// update time and elapsed time
	record_time();

	return true;
}

void minty::Engine::stop()
{
	if (_running)
	{
		exit(0);
	}
}

void minty::Engine::cleanup()
{
	// wait for Vulkan to sync so no thread racing
	_renderEngine->sync();

	// clean up scenes
	_sceneManager->unload();
	_sceneManager->destroy();
}

void minty::Engine::destroy()
{
	if (_window) delete _window;
	if (_renderEngine) delete _renderEngine;
	if (_audioEngine) delete _audioEngine;
	if (_sceneManager) delete _sceneManager;
}

void minty::Engine::record_time()
{
	// get the current time
	TimePoint now = Time::now();

	// calculate the times in seconds
	_time.time = Time::calculate_duration_seconds(_time.start, now);
	_time.elapsed = Time::calculate_duration_seconds(_time.end, now);

	_time.end = now;
}

String minty::to_string(Engine const& value)
{
	return std::format("Engine()");
}
