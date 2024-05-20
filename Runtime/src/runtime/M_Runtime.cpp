#include "pch.h"
#include "runtime/M_Runtime.h"

#include "input/M_InputMap.h"
#include "scenes/M_SceneManager.h"
#include "runtime/M_Info.h"
#include "tools/M_Console.h"
#include "scenes/M_Scene.h"

#include "assets/M_AssetEngine.h"
#include "rendering/M_RenderEngine.h"
#include "audio/M_AudioEngine.h"
#include "scripting/M_ScriptEngine.h"
#include "systems/M_SystemRegistry.h"
#include "animation/M_AnimationSystem.h"
#include "audio/M_AudioSystem.h"
#include "rendering/M_RenderSystem.h"
#include "scripting/M_ScriptSystem.h"
#include "ui/M_UISystem.h"

#include "entities/M_EntityRegistry.h"
#include "animation/M_AnimatorComponent.h"
#include "audio/M_AudioListenerComponent.h"
#include "audio/M_AudioSourceComponent.h"
#include "rendering/M_CameraComponent.h"
#include "ui/M_CanvasComponent.h"
#include "components/M_DirtyComponent.h"
#include "components/M_DestroyEntityComponent.h"
#include "components/M_EnabledComponent.h"
#include "rendering/M_MeshComponent.h"
#include "components/M_NameComponent.h"
#include "components/M_RelationshipComponent.h"
#include "rendering/M_RenderableComponent.h"
#include "scripting/M_ScriptComponent.h"
#include "rendering/M_SpriteComponent.h"
#include "components/M_TagComponent.h"
#include "components/M_TransformComponent.h"
#include "ui/M_UITransformComponent.h"

#include "libraries/M_GLFW.h"
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
		MINTY_ERROR(message);
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
	while (loop()) {}
}

bool minty::Runtime::loop()
{
	// stop running if state not Running
	if (_state != State::Running)
	{
		return false;
	}

	// stop running if window is closed
	if (!_window->is_open())
	{
		stop();
		return false;
	}

	// run window events and input
	_window->poll_events();

	if (_window->is_resized())
	{
		// window was resized, dirty all UI components so they update
		if (Scene* scene = _sceneManager->get_loaded_scene())
		{
			EntityRegistry& registry = scene->get_entity_registry();

			for (auto [entity, transform] : registry.view<UITransformComponent>().each())
			{
				registry.dirty(entity);
			}
		}
	}

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
	MINTY_LOG("Linking the Runtime:");

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
	register_component<CanvasComponent>(ASSEMBLY_ENGINE_NAME, "Canvas");
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
