#include "pch.h"
#include "M_System.h"

#include "M_Engine.h"

using namespace minty;

minty::System::System(Engine* const engine, EntityRegistry* const registry)
	: _enabled(true)
	, _engine(engine)
	, _registry(registry)
{
	console::log("System engine/registry addrs: " + std::to_string(reinterpret_cast<uintptr_t>(engine)) + "/" + std::to_string(reinterpret_cast<uintptr_t>(registry)), console::Color::Green);
}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

bool minty::System::is_enabled() const
{
	return _enabled;
}
