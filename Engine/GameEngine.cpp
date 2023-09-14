#include "GameEngine.h"

#include "RenderEngine.h"

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
