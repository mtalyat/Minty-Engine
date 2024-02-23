#include "pch.h"
#include "M_Engine.h"

using namespace minty;

minty::Engine::Engine(Runtime& runtime)
	: _runtime(&runtime)
	, _loadedScene()
	, _workingScene()
{}

minty::Engine::~Engine()
{
}

Runtime& minty::Engine::get_runtime() const
{
	return *_runtime;
}

void minty::Engine::set_loaded_scene(Scene* const scene)
{
	_loadedScene = scene;
}

Scene* minty::Engine::get_loaded_scene() const
{
	return _loadedScene;
}

void minty::Engine::set_working_scene(Scene* const scene)
{
	_workingScene = scene;
}

Scene* minty::Engine::get_working_scene() const
{
	return _workingScene;
}
