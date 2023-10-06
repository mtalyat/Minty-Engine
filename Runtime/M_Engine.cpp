#include "pch.h"
#include "M_Engine.h"

#include "M_Console.h"
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace minty;
using namespace minty::rendering;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Engine::Engine(Info const* const info)
	: _window("Minty", WIDTH, HEIGHT)
	, _renderer(&_window)
	, _sceneManager(this)
	, _deltaTime(0.02f)
{}

Engine::~Engine()
{}

Window* minty::Engine::get_window()
{
	return &_window;
}

Renderer* minty::Engine::get_renderer()
{
	return &_renderer;
}

SceneManager* minty::Engine::get_scene_manager()
{
	return &_sceneManager;
}

float minty::Engine::get_delta_time()
{
	return _deltaTime;
}

void Engine::run()
{
	// if no scenes at all, abort
	if (!_sceneManager.size())
	{
		throw std::runtime_error("Aborting game. No Scenes loaded.");
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
	while (_renderer.is_running())
	{
		// run window events
		glfwPollEvents();

		// update scene(s)
		_sceneManager.update();

		// update renderer
		_renderer.update();

		// render to the screen
		_renderer.render_frame();

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
			console::log(std::format("{}\r", fpsCount));

			fpsCount = 0u;
			fpsTime = 0ll;
		}
	}

	// all done
	_sceneManager.unload();

	// print elapsed time
	console::log(std::format("Elapsed time: {}s", std::chrono::duration_cast<std::chrono::milliseconds>(get_now() - start).count() / 1000.0f));

	// cleanup
	_renderer.destroy();
}

time_point_t minty::Engine::get_now() const
{
	return std::chrono::steady_clock::now();
}
