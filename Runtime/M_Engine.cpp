#include "pch.h"
#include "M_Engine.h"

using namespace minty;

minty::Engine::Engine()
	: _runtime()
{}

minty::Engine::~Engine()
{
}

void minty::Engine::set_runtime(Runtime& runtime)
{
	_runtime = &runtime;
}

Runtime& minty::Engine::get_runtime() const
{
	return *_runtime;
}

void minty::Engine::set_scene(Scene* scene)
{
	_scene = scene;
}

Scene* minty::Engine::get_scene() const
{
	return _scene;
}
