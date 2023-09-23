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

void minty::Scene::load()
{
	_systems.load();
}

void minty::Scene::update()
{
	_systems.update();
}

void minty::Scene::fixed_update()
{
	_systems.fixed_update();
}

void minty::Scene::unload()
{
	_systems.unload();
}
