#include "pch.h"
#include "M_System.h"

#include "M_Engine.h"

using namespace minty;

minty::System::System()
	: _enabled(true)
	, _engine()
	, _registry()
{
}

void minty::System::init(Engine& engine, EntityRegistry& registry)
{
	_engine = &engine;
	_registry = &registry;
}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

bool minty::System::is_enabled() const
{
	return _enabled;
}
