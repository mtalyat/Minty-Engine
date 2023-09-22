#include "pch.h"
#include "M_System.h"

using namespace minty;

minty::System::System(Registry* const registry)
	: _enabled(true)
	, _registry(registry)
{}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

constexpr bool minty::System::is_enabled() const
{
	return _enabled;
}
