#include "pch.h"
#include "M_GameEngine.h"

#include "M_RenderEngine.h"
#include <iostream>

using namespace minty;

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{

}

void GameEngine::run()
{
	RenderEngine engine;

	while (engine.isRunning())
	{
		glfwPollEvents();
		engine.renderFrame();
	}

	std::string str;
	std::getline(std::cin, str);
}
