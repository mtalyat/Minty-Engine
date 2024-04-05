#include "pch.h"
#include "M_Runtime.h"

#include "M_InputMap.h"
#include "M_SceneManager.h"
#include "M_Info.h"
#include "M_Console.h"

#include "M_AssetEngine.h"
#include "M_RenderEngine.h"
#include "M_AudioEngine.h"
#include "M_ScriptEngine.h"
#include "M_SystemRegistry.h"
#include "M_AnimationSystem.h"
#include "M_AudioSystem.h"
#include "M_RenderSystem.h"
#include "M_ScriptSystem.h"
#include "M_UISystem.h"

#include "M_EntityRegistry.h"
#include "M_AnimatorComponent.h"
#include "M_AudioListenerComponent.h"
#include "M_AudioSourceComponent.h"
#include "M_CameraComponent.h"
#include "M_DirtyComponent.h"
#include "M_DestroyEntityComponent.h"
#include "M_EnabledComponent.h"
#include "M_MeshComponent.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_RenderableComponent.h"
#include "M_ScriptComponent.h"
#include "M_SpriteComponent.h"
#include "M_TagComponent.h"
#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"

#include "M_GLFW.h"
#include <iostream>

using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Runtime::Runtime(Info const& info, RunMode const mode)
	: _mode(mode)
	, _state(State::Uninitialized)
	, _info(info)
	, _window()
	, _sceneManager()
	, _engines()
	, _frameCount()
	, _exitCode()
{}

Runtime::~Runtime()
{
	if (_state == State::Running)
	{
		stop();
		cleanup();
	}
	destroy();
}

RunMode minty::Runtime::get_mode() const
{
	return _mode;
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
	return _state == State::Running;
}

Window& minty::Runtime::get_window() const
{
	MINTY_ASSERT_MESSAGE(_state >= State::Initialized, "Runtime is not initialized.");

	return *_window;
}

AssetEngine& minty::Runtime::get_asset_engine() const
{
	return *static_cast<AssetEngine*>(_engines.at(ASSET_ENGINE_INDEX));
}

RenderEngine& minty::Runtime::get_render_engine() const
{
	return *static_cast<RenderEngine*>(_engines.at(RENDER_ENGINE_INDEX));
}

AudioEngine& minty::Runtime::get_audio_engine() const
{
	return *static_cast<AudioEngine*>(_engines.at(AUDIO_ENGINE_INDEX));
}

ScriptEngine& minty::Runtime::get_script_engine() const
{
	return *static_cast<ScriptEngine*>(_engines.at(SCRIPT_ENGINE_INDEX));
}

void minty::Runtime::set_loaded_scene(Scene* const scene) const
{
	for (auto const engine : _engines)
	{
		engine->set_loaded_scene(scene);
	}
}

void minty::Runtime::set_working_scene(Scene* const scene) const
{
	for (auto const engine : _engines)
	{
		engine->set_working_scene(scene);
	}
}

SceneManager& minty::Runtime::get_scene_manager() const
{
	MINTY_ASSERT_MESSAGE(_state >= State::Initialized, "Runtime is not initialized.");

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
	if (_state == State::Running) _state = State::Stopped;
}

bool minty::Runtime::init(Window& window, RuntimeBuilder const* builder)
{
	// already initialized
	if (_state > State::Uninitialized) return true;

	_state = State::Initialized;

	// create necessary components
	_window = &window;
	_sceneManager = new SceneManager(*this);

	set_engine<AssetEngine>(builder && builder->assetEngine ? builder->assetEngine : new AssetEngine(*this));
	set_engine<RenderEngine>(builder && builder->renderEngine ? builder->renderEngine : new RenderEngine(*this));
	set_engine<AudioEngine>(builder && builder->audioEngine ? builder->audioEngine : new AudioEngine(*this));
	set_engine<ScriptEngine>(builder && builder->scriptEngine ? builder->scriptEngine : new ScriptEngine(*this));

	return true;
}

bool minty::Runtime::start()
{
	if (_state == State::Running) return true; // already started

	_state = State::Running;

	// if there are scenes, load them
	if (_sceneManager->size())
	{
		// if no scene loaded, just load the first scene
		if (!_sceneManager->get_loaded_scene())
		{
			_sceneManager->load_scene(0);
		}
	}

	// start the scene(s)
	_sceneManager->load();

	// get the recorded time
	_time.start = Time::now();
	_time.end = _time.start;
	record_time();

	return true;
}

void Runtime::run()
{
	while (_state == State::Running && loop()) {}
}

bool minty::Runtime::loop()
{
	// stop running if window is closed
	if (!_window->is_open())
	{
		stop();
		return false;
	}

	// run window events and input
	_window->poll_events();

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
	if (_state == State::Running)
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
	if (_state == State::Destroyed) return;

	_state = State::Destroyed;

	MINTY_DELETE(_sceneManager);

	// destroy asset engine, then others
	AssetEngine* assets = _engines.at<AssetEngine>();
	delete assets;

	// set to null so it is not deleted later
	_engines.erase<AssetEngine>();

	// delete others
	for (auto const engine : _engines)
	{
		if(engine) delete engine;
	}
	_engines.clear();
}

void minty::Runtime::register_script(String const& name)
{
	EntityRegistry::register_script(name);
	ScriptEngine::link_script(name);
}

void minty::Runtime::link()
{
	Console::log("Linking the Runtime:");

	// systems
	register_system<AnimationSystem>("Animation");
	register_system<AudioSystem>("Audio");
	register_system<RenderSystem>("Render");
	register_system<ScriptSystem>("Script");
	register_system<UISystem>("UI");

	// components
	register_component<AnimatorComponent>(ASSEMBLY_ENGINE_NAME, "Animator");
	register_component<AudioListenerComponent>(ASSEMBLY_ENGINE_NAME, "AudioListener");
	register_component<AudioSourceComponent>(ASSEMBLY_ENGINE_NAME, "AudioSource");
	register_component<CameraComponent>(ASSEMBLY_ENGINE_NAME, "Camera");
	//register_component<DestroyComponent>(ASSEMBLY_ENGINE_NAME, "Destroy");
	//register_component<DirtyComponent>(ASSEMBLY_ENGINE_NAME, "Dirty");
	register_component<EnabledComponent>(ASSEMBLY_ENGINE_NAME, "Enabled", false);
	register_component<MeshComponent>(ASSEMBLY_ENGINE_NAME, "Mesh");
	register_component<NameComponent>(ASSEMBLY_ENGINE_NAME, "Name");
	register_component<RelationshipComponent>(ASSEMBLY_ENGINE_NAME, "Relationship");
	register_component<RenderableComponent>(ASSEMBLY_ENGINE_NAME, "Renderable");
	register_component<ScriptComponent>(ASSEMBLY_ENGINE_NAME, "Script");
	register_component<SpriteComponent>(ASSEMBLY_ENGINE_NAME, "Sprite");
	register_component<TransformComponent>(ASSEMBLY_ENGINE_NAME, "Transform");
	register_component<UITransformComponent>(ASSEMBLY_ENGINE_NAME, "UITransform");

	// link to C#
	ScriptEngine::link();

	// link C++ scripting handles
	ScriptEngine& scriptEngine = get_script_engine();

	// window
	_window->on_link(scriptEngine);
}

void minty::Runtime::record_time()
{
	// get the current time
	TimePoint now = Time::now();

	// calculate the times in seconds
	_time.total = Time::calculate_duration_seconds(_time.start, now);
	_time.elapsed = Time::calculate_duration_seconds(_time.end, now);

	_time.end = now;
}

String minty::to_string(Runtime const& value)
{
	return std::format("Runtime()");
}
