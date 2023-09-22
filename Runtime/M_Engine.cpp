#include "pch.h"
#include "M_Engine.h"

#include "M_Console.h"
#include "M_Renderer.h"
#include <iostream>

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

constexpr Window& minty::Engine::getWindow()
{
	return _window;
}

void Engine::run()
{
	// record start time, and last frame tick
	time_point_t start = getNow();
	time_point_t frameTick = start;
	unsigned int frameCount = 0u;

	Window* windowPtr = &_window;

	// start the render engine
	Renderer renderer(windowPtr, *this);
	Renderer* rendererPtr = &renderer;

	

	time_point_t now;

	// main loop
	while (renderer.isRunning())
	{
		glfwPollEvents();
		renderer.updateUniformBuffer();
		renderer.renderFrame();

		frameCount++;

		now = getNow();

		// if frame tick >= 1 second
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameTick).count() >= 1000000000ll)
		{
			std::cout << frameCount << '\r' << std::endl;

			frameCount = 0u;
			frameTick = now;
		}
	}

	// print elapsed time
	std::cout << "Elapsed time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(getNow() - start).count() / 1000.0f) << "s" << std::endl;
}

time_point_t minty::Engine::getNow() const
{
	return std::chrono::steady_clock::now();
}
