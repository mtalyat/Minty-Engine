#include "pch.h"
#include "Application.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Console.h"
#include "Minty/Core/Linker.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pack.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Time.h"
#include "Minty/Core/Window.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Renderer.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Script/ScriptEngine.h"
#include <thread>

using namespace Minty;

Application* Application::sp_instance = nullptr;

Minty::Application::Application(const ApplicationBuilder& builder)
	: m_running(false)
	, m_minimized(false)
	, m_time()
	, m_targetFPS(builder.targetFPS)
	, m_info(builder.info)
	, m_mode(builder.mode)
	, m_logger(builder.logPath, true)
	, m_window()
	, m_sceneManager()
	, m_workingScenes()
{
	MINTY_ASSERT_MESSAGE(builder.targetFPS > 0, "Application target FPS must be above zero.");

	MINTY_ASSERT(sp_instance == nullptr);
	sp_instance = this;

	// create window
	WindowBuilder windowBuilder{};
	m_window = Window::create(windowBuilder);
	m_window->set_event_callback([this](Event& e) { on_event(e); });

	// initialize tools

	//  asset manager
	AssetManagerBuilder assetManagerBuilder{};
	assetManagerBuilder.mode = AssetMode::ReadAll;
	assetManagerBuilder.wraps = { "default.wrap", "game.wrap" };
	AssetManager::initialize(assetManagerBuilder);

	//  renderer
	RendererBuilder rendererBuilder{};
	rendererBuilder.window = m_window;
	Renderer::initialize(rendererBuilder);

	//	script engine
	ScriptEngineBuilder scriptEngineBuilder{};
	ScriptEngine::initialize(scriptEngineBuilder);

	// load MintyEngine assembly
	ScriptEngine::load_assembly(MINTY_NAME_ENGINE, "Lib/MintyEngine.dll", builder.mode == ApplicationMode::Edit);

	// glue everything together
	Linker::link();
}

Minty::Application::~Application()
{
	// shutdown tools, in reverse order
	AssetManager::shutdown();
	Renderer::shutdown();

	MINTY_ASSERT(sp_instance != nullptr);
	sp_instance = nullptr;
}

Int Application::run()
{
	MINTY_ASSERT_MESSAGE(m_running == false, "Attempting to run the Application when it is already running.");
	m_running = true;

	// load the initial scene
	Container* container;
	Reader* reader;
	AssetManager::open_reader("game.appdata", container, reader);

	Path scenePath;
	if (!reader->read("InitialScene", scenePath))
	{
		// no initialScene OR scene at path DNE
		MINTY_ABORT("No initial Scene given.");
	}

	AssetManager::close_reader(container, reader);

	MINTY_ASSERT_FORMAT(AssetManager::exists(scenePath), "No initial Scene found at path \"{}\".", scenePath.generic_string());

	// load it
	m_sceneManager.load(scenePath);

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

	Owner<RenderTarget> target = Renderer::create_render_target();
	Renderer::set_render_target(target);

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
		m_window->process();

		if (!m_minimized)
		{
			if (Renderer::start_frame())
			{
				// TODO: still run game code, just don't render
				// skip this frame
				continue;
			}

			// update scene
			m_sceneManager.update(m_time);

			// finalize scene
			m_sceneManager.finalize();

			Renderer::end_frame();
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
	m_running = false;
}

void Minty::Application::on_event(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_close));
	dispatcher.dispatch<WindowResizeEvent>(MINTY_BIND_EVENT_FUNCTION(Application::on_window_resize));
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