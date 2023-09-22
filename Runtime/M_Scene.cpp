#include "pch.h"
#include "M_Scene.h"

using namespace minty;

minty::Scene::Scene()
	: _entities()
	, _systems()
{
}

EntityRegistry& minty::Scene::get_entity_registry()
{
	return _entities;
}

SystemRegistry& minty::Scene::get_system_registry()
{
	return _systems;
}
