#include "pch.h"
#include "M_System.h"

#include "M_Runtime.h"
#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

using namespace minty;

minty::System::System(String const& name, Runtime& engine, ID const sceneId)
	: SceneObject(engine, sceneId)
	, _enabled(true)
	, _name(name)
{}

minty::System::~System()
{}

String const& minty::System::get_name() const
{
	return _name;
}

EntityRegistry& minty::System::get_entity_registry() const
{
	return get_scene().get_entity_registry();
}

SystemRegistry& minty::System::get_system_registry() const
{
	return get_scene().get_system_registry();
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
