#include "pch.h"
#include "Minty/Systems/M_System.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

#include "Minty/Serialization/M_Reader.h"

using namespace Minty;

EntityRegistry& Minty::System::get_entity_registry() const
{
	return get_scene().get_entity_registry();
}

SystemRegistry& Minty::System::get_system_registry() const
{
	return get_scene().get_system_registry();
}
