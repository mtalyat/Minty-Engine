#include "pch.h"
#include "M_Engine.h"

#include "M_Console.h"
#include "M_Renderer.h"
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Engine::Engine()
	: _window("Minty", WIDTH, HEIGHT)
	, _renderer(&_window)
{

}

Engine::~Engine()
{

}

Window& minty::Engine::get_window()
{
	return _window;
}

Renderer& minty::Engine::get_renderer()
{
	return _renderer;
}

SceneManager& minty::Engine::get_scene_manager()
{
	return _sceneManager;
}

void Engine::run()
{
	// start the renderer
	_renderer.start();

	// start the scene(s)


	// record start time, and last frame tick
	time_point_t start = get_now();
	time_point_t frameTick = start;
	unsigned int frameCount = 0u;

	time_point_t now;

	// main loop
	while (_renderer.running())
	{
		glfwPollEvents();
		_renderer.update_uniform_buffer();
		_renderer.renderFrame();

		frameCount++;

		now = get_now();

		// if frame tick >= 1 second
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameTick).count() >= 1000000000ll)
		{
			std::cout << frameCount << '\r' << std::endl;

			frameCount = 0u;
			frameTick = now;
		}
	}

	// print elapsed time
	std::cout << "Elapsed time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(get_now() - start).count() / 1000.0f) << "s" << std::endl;
}

time_point_t minty::Engine::get_now() const
{
	return std::chrono::steady_clock::now();
}
