#include "pch.h"
#include "scenes/M_SceneObject.h"
#include "scenes/M_Scene.h"

using namespace minty;

minty::SceneObject::SceneObject(Runtime& runtime, Scene& scene)
	: RuntimeObject(runtime)
	, _scene(&scene)
{}

Scene& minty::SceneObject::get_scene() const
{
	return *_scene;
}

EntityRegistry& minty::SceneObject::get_entity_registry() const
{
	return _scene->get_entity_registry();
}

SystemRegistry& minty::SceneObject::get_system_registry() const
{
	return _scene->get_system_registry();
}

void minty::SceneObject::set_scene(Scene& scene)
{
	_scene = &scene;
}
