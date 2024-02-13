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
