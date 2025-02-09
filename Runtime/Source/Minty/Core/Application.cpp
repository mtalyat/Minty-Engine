#include "pch.h"
#include "Application.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Console.h"
#include "Minty/Core/Linker.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pack.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Time.h"
#include "Minty/Window/Window.h"
#include "Minty/Input/Input.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Renderer.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Script/ScriptEngine.h"
#include <thread>

using namespace Minty;

Application* Application::sp_instance = nullptr;

void Minty::Application::initialize(ApplicationBuilder const& builder)
{
	MINTY_ASSERT_MESSAGE(!m_initialized, "Application is already initialized. Must shutdown before re-initializing.");

	m_initialized = true;

	MINTY_ASSERT(sp_instance == nullptr);
	sp_instance = this;

	// setup
	MINTY_ASSERT_MESSAGE(builder.targetFPS > 0, "Application target FPS must be above zero.");
	m_targetFPS = builder.targetFPS;
	m_info = builder.info;
	m_data = builder.data;
	m_mode = builder.mode;
	m_passFlags = builder.passes;

	// initialize tools

	//  asset manager
	AssetManager::initialize(builder.assetManagerBuilder);

	//	script engine
	ScriptEngine::initialize(builder.scriptEngineBuilder);

	// load Minty assembly
	ScriptEngine::load_assembly(MINTY_NAME_ENGINE, "MintyEngine.dll", builder.mode == ApplicationMode::Edit);

	// glue everything together
	Linker::link();

	// load game data
	if (!load_data())
	{
		MINTY_ERROR("Failed to load data.");
	}

	// input
	Input::initialize(builder.inputBuilder);

	// window manager
	WindowManager::initialize(builder.windowManagerBuilder);

	// window
	WindowBuilder windowBuilder{};
	windowBuilder.id = UUID::create();
	windowBuilder.title = builder.info.name;
	Ref<Window> window = WindowManager::create_window(windowBuilder);
	window->set_event_callback([this](Event& e) { on_event(e); });

	//  renderer
	RendererBuilder rendererBuilder = builder.rendererBuilder;
	rendererBuilder.window = window;
	Renderer::initialize(rendererBuilder);

	// gui, if it is enabled
	if (has_pass_flag(ApplicationPassFlags::Gui))
	{
		GUI::initialize(builder.guiBuilder);
	}
}

void Minty::Application::shutdown()
{
	MINTY_ASSERT_MESSAGE(m_initialized, "Cannot shutdown an Application that has not yet been initialized.");

	// shutdown tools
	if (has_pass_flag(ApplicationPassFlags::Gui))
	{
		GUI::shutdown();
	}
	AssetManager::shutdown();
	Renderer::shutdown();
	Input::shutdown();
	WindowManager::shutdown();
	ScriptEngine::shutdown();

	// destroy resources
	MINTY_ASSERT(sp_instance != nullptr);
	sp_instance = nullptr;

	m_initialized = false;
}

Int Application::run()
{
	MINTY_ASSERT_MESSAGE(m_initialized, "Attempting to run the Application when it has not yet been initialized.");

	MINTY_ASSERT_MESSAGE(m_running == false, "Attempting to run the Application when it is already running.");
	m_running = true;

	load_initial_scene();

	// start time
	TimePoint now = Time::now();
	TimePoint start = now;
	TimePoint end = now;

	// keep track of elapsed for update functions
	m_time.total = 0.0f;
	m_time.elapsed = 0.0f;

	// fps tracking/management
	Float const fpsTargetTime = 1.0f / m_targetFPS;
	Size frames = 0;
	TimePoint fpsTime = now;

	// get window
	Ref<Window> window = WindowManager::get_main();

	// TODO: make different loops for each pass combination so it does not have to do all these comparisons every frame
	while (m_running)
	{
		// get current time
		now = Time::now();

		// sleep if needed
		Float elapsed = Time::calculate_duration_seconds(end, now);
		if (elapsed < fpsTargetTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<TimeElapsed>((fpsTargetTime - elapsed) * 1000.0f)));
		}

		// update time for update events
		now = Time::now();
		m_time.total = Time::calculate_duration_seconds(start, now);
		m_time.elapsed = Time::calculate_duration_seconds(end, now);
		end = now;

		// print FPS if needed
		if (Time::calculate_duration_seconds(fpsTime, now) >= 1.0f)
		{
			fpsTime = now;
			Console::log(to_string(frames));
			frames = 0;
		}

		// process window events
		window->process();

		if (Renderer::start_frame())
		{
			// TODO: still run game code, just don't render
			// TODO: quit on certain exit codes
			// skip this frame
			continue;
		}

		// update scene
		m_sceneManager.update(m_time);

		// finalize scene
		m_sceneManager.finalize();

		// update application
		update(m_time);

		if (has_pass_flag(ApplicationPassFlags::Scene))
		{
			// draw scene
			m_sceneManager.draw();
		}

		// if both passes, transition the assets
		if (has_pass_flag(ApplicationPassFlags::All))
		{
			Renderer::transition_between_render_passes();
		}

		if (has_pass_flag(ApplicationPassFlags::Gui))
		{
			Renderer::start_render_pass(GUI::get_render_pass(), GUI::get_render_target());

			if (GUI::start_frame())
			{
				// TODO: quit on certain exit codes
				// skip this frame
				Renderer::end_frame();
				continue;
			}

			update_gui(m_time);

			GUI::end_frame();

			Renderer::end_render_pass();
		}

		Renderer::end_frame();

		// unload assets that are marked for delayed unloading
		if (AssetManager::ready_to_collect())
		{
			// wait until frame is done rendering
			Renderer::sync();

			// destroy asset
			AssetManager::collect();
		}

		frames++;
	}

	// sync to avoid errors on close
	Renderer::sync();

	// unload any scene
	m_sceneManager.unload();

	return 0;
}

void Minty::Application::close()
{
	MINTY_ASSERT_MESSAGE(m_initialized, "Attempting to close the Application when it has not yet been initialized.");
	m_running = false;
}

void Minty::Application::on_event(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_close));
	dispatcher.dispatch<WindowResizeEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_resize));

	// these events are always called
	switch (event.get_event_type())
	{
	case EventType::WindowResize:
	{
		// TODO: move this out of this function into more natural place

		Ref<Scene> scene = m_sceneManager.get_working_scene();
		if (!scene) break;

		//// resize UI elements
		//EntityRegistry& registry = scene->get_entity_registry();

		//for (auto [entity, transform] : registry.view<UITransformComponent>().each())
		//{
		//	registry.dirty(entity);
		//}

		break;
	}
	}

	// these events only called in normal run mode
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		switch (event.get_event_type())
		{
		case EventType::Key:
		{
			KeyEvent& e = static_cast<KeyEvent&>(event);
			Input::trigger_key(e.get_key(), e.get_key_action(), e.get_key_modifiers());
			break;
		}
		case EventType::MouseButton:
		{
			MouseButtonEvent& e = static_cast<MouseButtonEvent&>(event);
			Input::trigger_mouse_button(e.get_button(), e.get_action(), e.get_key_modifiers());
			break;
		}
		case EventType::MouseMoved:
		{
			MouseMovedEvent& e = static_cast<MouseMovedEvent&>(event);
			Input::trigger_mouse_move(e.get_x(), e.get_y());
			break;
		}
		case EventType::MouseScrolled:
		{
			MouseScrolledEvent& e = static_cast<MouseScrolledEvent&>(event);
			Input::trigger_mouse_scroll(e.get_delta_x(), e.get_delta_y());
			break;
		}
		case EventType::GamepadConnected:
		{
			GamepadConnectedEvent& e = static_cast<GamepadConnectedEvent&>(event);
			Input::trigger_gamepad_connect(e.get_controller());
			break;
		}
		case EventType::GamepadDisconnected:
		{
			GamepadDisconnectedEvent& e = static_cast<GamepadDisconnectedEvent&>(event);
			Input::trigger_gamepad_disconnect(e.get_controller());
			break;
		}
		case EventType::GamepadButton:
		{
			GamepadButtonEvent& e = static_cast<GamepadButtonEvent&>(event);
			Input::trigger_gamepad_button(e.get_controller(), e.get_button(), e.get_action());
			break;
		}
		case EventType::GamepadAxis:
		{
			GamepadAxisEvent& e = static_cast<GamepadAxisEvent&>(event);
			Input::trigger_gamepad_axis(e.get_controller(), e.get_axis(), e.get_value());
			break;
		}
		}
	}
}

Bool Minty::Application::load_data()
{
	// load Game.wrap
	if (!load_game_wrap())
	{
		MINTY_ERROR("Failed to load game wrap.");
		return false;
	}

	// load appdata
	if (!load_game_appdata())
	{
		MINTY_ERROR("Failed to load game application data.");
		return false;
	}

	// load config
	if (!load_game_config())
	{
		MINTY_ERROR("Failed to load game config data.");
		return false;
	}

	return true;
}

Bool Minty::Application::clear_config()
{
	m_layerManager.clear();

	return true;
}

Bool Minty::Application::load_config(Path const& path)
{
	Reader* reader;
	if (!AssetManager::open_reader(path, reader))
	{
		// could not open app data
		return false;
	}

	// load layers
	if (reader->indent("Layers"))
	{
		m_layerManager.deserialize(*reader);

		reader->outdent();
	}

	AssetManager::close_reader(reader);
	return true;
}

Bool Minty::Application::load_game_wrap()
{
	return AssetManager::load_wrap(DEFAULT_GAME_WRAP);
}

Bool Minty::Application::load_game_appdata()
{
	// read Game.appdata
	Reader* reader;
	if (!AssetManager::open_reader(DEFAULT_GAME_APPDATA, reader))
	{
		// could not open app data
		return false;
	}

	reader->read("InitialScene", m_data.initialScene);
	reader->read("Assemblies", m_data.assemblies);
	reader->read("Wraps", m_data.wraps);

	// load the other assemblies
	for (Path const& assemblyPath : m_data.assemblies)
	{
		String assemblyName = assemblyPath.stem().string();
		Ref<ScriptAssembly> assembly = ScriptEngine::load_assembly(assemblyName, assemblyPath);
		if (assembly == nullptr)
		{
			MINTY_ERROR_FORMAT("Failed to load assembly at path \"{}\".", assemblyPath.generic_string());
			return false;
		}
	}

	// load the other wraps
	for (Path const& wrapPath : m_data.wraps)
	{
		if (!AssetManager::load_wrap(wrapPath))
		{
			MINTY_ERROR_FORMAT("Failed to load wrap at path \"{}\".", wrapPath.generic_string());
			return false;
		}
	}

	AssetManager::close_reader(reader);
	return true;
}

Bool Minty::Application::load_game_config()
{
	return load_config(DEFAULT_GAME_CONFIG);
}

Bool Minty::Application::load_initial_scene()
{
	if (m_data.initialScene.empty())
	{
		// no scene specified
		return false;
	}

	// if scene was given, make sure it exists
	MINTY_ASSERT_FORMAT(AssetManager::exists(m_data.initialScene), "No initial Scene found at path \"{}\".", m_data.initialScene.generic_string());

	// load the scene
	m_sceneManager.load(m_data.initialScene);

	return true;
}

Bool Minty::Application::on_window_close(WindowCloseEvent& event)
{
	close();
	return true;
}

Bool Minty::Application::on_window_resize(WindowResizeEvent& event)
{
	if (event.get_width() == 0 || event.get_height() == 0)
	{
		m_minimized = true;
		return false;
	}

	m_minimized = false;
	// TODO: SEND EVENT TO RENDERER

	return false;
}