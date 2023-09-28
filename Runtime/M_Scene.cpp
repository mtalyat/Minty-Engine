#include "pch.h"
#include "M_Scene.h"

#include "M_Engine.h"

using namespace minty;

minty::Scene::Scene(Engine* const engine)
	: _engine(engine)
	, _entities(new EntityRegistry())
	, _systems(new SystemRegistry(engine, _entities))
{}

minty::Scene::~Scene()
{
	delete _entities;
	_entities = nullptr;
	delete _systems;
	_systems = nullptr;
}

minty::Scene::Scene(Scene&& other) noexcept
	: _engine(other._engine)
	, _entities(other._entities)
	, _systems(other._systems)
{
	other._engine = nullptr;
	other._entities = nullptr;
	other._systems = nullptr;
}

Scene& minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		_engine = other._engine;
		_entities = other._entities;
		_systems = other._systems;

		other._engine = nullptr;
		other._entities = nullptr;
		other._systems = nullptr;
	}

	return *this;
}

EntityRegistry* minty::Scene::get_entity_registry() const
{
	return _entities;
}

SystemRegistry* minty::Scene::get_system_registry() const
{
	return _systems;
}

void minty::Scene::load()
{
	_systems->load();
}

void minty::Scene::update()
{
	_systems->update();
}

void minty::Scene::fixed_update()
{
	_systems->fixed_update();
}

void minty::Scene::unload()
{
	_systems->unload();
}

std::string const minty::Scene::to_string() const
{
	return std::format("Scene({}, {})", _systems->to_string(), _entities->to_string());
}

void minty::Scene::serialize(Writer& writer) const
{
	writer.write("Systems", _systems);
	writer.write("Entities", _entities);
}

void minty::Scene::deserialize(Reader const& reader)
{
	reader.read_object("Systems", _systems);
	reader.read_object("Entities", _entities);
}
