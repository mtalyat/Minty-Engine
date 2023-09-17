#include "pch.h"
#include "M_GameEngine.h"

#include "M_Console.h"
#include "M_RenderEngine.h"
#include <iostream>

using namespace minty;

GameEngine::GameEngine()
	: _start()
	, _frameTick()
	, _frameCount()
{

}

GameEngine::~GameEngine()
{
}

void GameEngine::run()
{
	// record start time, and last frame tick
	_start = getNow();
	_frameTick = _start;

	// start the render engine
	RenderEngine engine(*this);

	float rotation = 0.0f;
	float* rotationPtr = &rotation;

	// input
	InputMap input;
	input.emplaceKey(Key::Space, [rotationPtr](KeyPressEventArgs const& args){
		switch (args.action)
		{
		case KeyAction::Down:
			break;
		case KeyAction::Up:
			break;
		case KeyAction::Hold:
			*rotationPtr += 0.01f;
			console::log(std::format("Rotation: {}", *rotationPtr));
			break;
		}
	});

	engine._window->setInput(&input);

	time_point_t now;

	// main loop
	while (engine.isRunning())
	{
		glfwPollEvents();
		engine.updateUniformBuffer(rotation);
		engine.renderFrame();

		_frameCount++;

		now = getNow();

		// if frame tick >= 1 second
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(now - _frameTick).count() >= 1000000000ll)
		{
			std::cout << _frameCount << '\r' << std::endl;

			_frameCount = 0u;
			_frameTick = now;
		}
	}

	// print elapsed time
	std::cout << "Elapsed time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(getNow() - _start).count() / 1000.0f) << "s" << std::endl;
}

time_point_t minty::GameEngine::getNow() const
{
	return std::chrono::steady_clock::now();
}
