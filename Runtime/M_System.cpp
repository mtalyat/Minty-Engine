#include "pch.h"
#include "M_System.h"

#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

using namespace minty;

minty::System::System(Scene& scene)
	: _enabled(true)
	, _scene(&scene)
{}

minty::System::~System()
{}

Scene& minty::System::get_scene() const
{
	return *_scene;
}

EntityRegistry& minty::System::get_entity_registry() const
{
	return _scene->get_entity_registry();
}

SystemRegistry& minty::System::get_system_registry() const
{
	return _scene->get_system_registry();
}

void minty::System::reset()
{}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

bool minty::System::is_enabled() const
{
	return _enabled;
}

String minty::to_string(System const& value)
{
	return std::format("System()");
}
