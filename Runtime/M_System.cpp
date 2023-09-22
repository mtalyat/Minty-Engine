#include "pch.h"
#include "M_System.h"

using namespace minty;

minty::System::System(Registry* const registry)
	: _enabled(true)
	, _registry(registry)
{}

void minty::System::enable()
{
	_enabled = true;
}

void minty::System::disable()
{
	_enabled = false;
}

void minty::System::setEnabled(bool const enabled)
{
	_enabled = enabled;
}

constexpr bool minty::System::isEnabled() const
{
	return _enabled;
}
