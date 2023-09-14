#include "pch.h"
#include "M_GameEngine.h"

#include "M_RenderEngine.h"

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
	engine.run();
}
