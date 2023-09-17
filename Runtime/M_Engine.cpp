#include "pch.h"
#include "M_Engine.h"

#include "M_Console.h"
#include "M_Renderer.h"
#include <iostream>

using namespace minty;

uint32_t const WIDTH = 800;
uint32_t const HEIGHT = 600;

Engine::Engine()
{
	// init GLFW
	glfwInit();
}

Engine::~Engine()
{
	// close GLFW
	glfwTerminate();
}

void Engine::run()
{
	// record start time, and last frame tick
	time_point_t start = getNow();
	time_point_t frameTick = start;
	unsigned int frameCount = 0u;

	Window window("Minty", WIDTH, HEIGHT);
	Window* windowPtr = &window;

	// start the render engine
	Renderer engine(windowPtr, *this);

	float rotation = 0.0f;
	float* rotationPtr = &rotation;

	// input
	InputMap input;
	// rotate when space held
	input.emplaceKey(Key::Space, [rotationPtr](KeyPressEventArgs const& args)
		{
			switch (args.action)
			{
			case KeyAction::Down:
				break;
			case KeyAction::Up:
				break;
			case KeyAction::Hold:
				if (static_cast<int>(args.mods) & static_cast<int>(KeyModifiers::Shift))
				{
					// rotate backwards
					*rotationPtr -= 0.01f;
				}
				else
				{
					// rotate forwards
					*rotationPtr += 0.01f;
				}
				
				break;
			}
		});
	// quit on key close
	input.emplaceKeyDown(Key::Escape, [windowPtr](KeyPressEventArgs const& args)
		{
			windowPtr->close();
		});

	window.setInput(&input);

	time_point_t now;

	// main loop
	while (engine.isRunning())
	{
		glfwPollEvents();
		engine.updateUniformBuffer(rotation);
		engine.renderFrame();

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
