#include "pch.h"
#include "M_System.h"

#include "M_Engine.h"

using namespace minty;

minty::System::System(Engine* const engine, EntityRegistry* const registry)
	: _enabled(true)
	, _engine(engine)
	, _registry(registry)
{}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

bool minty::System::is_enabled() const
{
	return _enabled;
}

std::string minty::to_string(System const& value)
{
	return std::format("System()");
}
