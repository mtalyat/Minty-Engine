#include "pch.h"
#include "systems/M_System.h"

#include "runtime/M_Runtime.h"
#include "assets/M_AssetEngine.h"
#include "scenes/M_Scene.h"
#include "entities/M_EntityRegistry.h"
#include "systems/M_SystemRegistry.h"

#include "serialization/M_Reader.h"
#include "serialization/M_Writer.h"

#include "serialization/M_Reader.h"

using namespace minty;

minty::System::System(String const& name, Runtime& engine, Scene& scene)
	: SceneObject(engine, scene)
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

void minty::System::load()
{}

void minty::System::unload()
{}

void minty::System::serialize(Writer& writer) const
{}

void minty::System::deserialize(Reader const& reader)
{}

String minty::to_string(System const& value)
{
	return std::format("System()");
}
