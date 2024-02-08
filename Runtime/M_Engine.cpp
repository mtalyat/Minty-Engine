#include "pch.h"
#include "M_Engine.h"

#include "M_Console.h"
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace minty;
using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Engine::Engine(Info const& info)
	: _globalInput()
	, _window(info.get_application_name(), WIDTH, HEIGHT, &_globalInput)
	, _renderEngine(&_window)
	, _audioEngine()
	, _sceneManager(*this)
	, _deltaTime(0.02f)
{}

Engine::~Engine()
{}

InputMap& minty::Engine::get_global_input_map()
{
	return _globalInput;
}

InputMap const& minty::Engine::get_global_input_map() const
{
	return _globalInput;
}

Window& minty::Engine::get_window()
{
	return _window;
}

Window const& minty::Engine::get_window() const
{
	return _window;
}

RenderEngine& minty::Engine::get_render_engine()
{
	return _renderEngine;
}

RenderEngine const& minty::Engine::get_render_engine() const
{
	return _renderEngine;
}

AudioEngine& minty::Engine::get_audio_engine()
{
	return _audioEngine;
}

AudioEngine const& minty::Engine::get_audio_engine() const
{
	return _audioEngine;
}

SceneManager& minty::Engine::get_scene_manager()
{
	return _sceneManager;
}

SceneManager const& minty::Engine::get_scene_manager() const
{
	return _sceneManager;
}

float minty::Engine::get_delta_time() const
{
	return _deltaTime;
}

void Engine::run()
{
	// if no scenes at all, abort
	if (!_sceneManager.size())
	{
		Error::abort("Aborting game. No Scenes loaded.");
	}

	// if no scene loaded, just load the first scene
	if (!_sceneManager.get_loaded_scene())
	{
		_sceneManager.load_scene(0);
	}

	// start the scene(s)
	_sceneManager.load();

	// record start time, and last frame tick
	time_point_t start = get_now();

	time_point_t frameTick = start;
	long long frameTime;

	long long fpsTime = 0ll;
	unsigned int fpsCount = 0u;

	time_point_t now;

	// main loop
	while (_renderEngine.is_running())
	{
		// run window events
		glfwPollEvents();

		// update scene(s)
		_sceneManager.update();

		// update audio engine
		//_audioEngine.update();

		// update renderer
		//_renderEngine.update();

		// cleanup scene
		_sceneManager.finalize();

		// render to the screen
		_renderEngine.render_frame();

		// frame complete
		fpsCount++;

		// get time right now
		now = get_now();

		// calculate time passed in nanoseconds
		frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameTick).count();

		// update delta time
		_deltaTime = frameTime / 1000000000.0f;

		// update frame tick to now, for next frame
		frameTick = now;

		// add to fps time
		fpsTime += frameTime;

		// if fps time >= 1 second (in nanoseconds)
		if (fpsTime >= 1000000000ll)
		{
			Console::log(std::format("{}\r", fpsCount));

			fpsCount = 0u;
			fpsTime = 0ll;
		}
	}

	// wait for render engine to finish before cleaning up
	_renderEngine.sync();

	// clean up
	_sceneManager.unload();
	_sceneManager.destroy();

	// print elapsed time of program
	Console::log(std::format("Elapsed time: {}s", std::chrono::duration_cast<std::chrono::milliseconds>(get_now() - start).count() / 1000.0f));
}

time_point_t minty::Engine::get_now() const
{
	return std::chrono::steady_clock::now();
}

String minty::to_string(Engine const& value)
{
	return std::format("Engine()");
}
