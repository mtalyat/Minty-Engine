#include "pch.h"
#include "M_SceneObject.h"

#include "M_Runtime.h"
#include "M_Scene.h"

using namespace minty;

minty::SceneObject::SceneObject()
	: _runtime()
	, _sceneId()
{}

minty::SceneObject::SceneObject(Runtime& engine, ID const sceneId)
	: _runtime(&engine)
	, _sceneId(sceneId)
{}

minty::SceneObject::~SceneObject()
{
}

Runtime& minty::SceneObject::get_runtime() const
{
	MINTY_ASSERT(_runtime != nullptr, "SceneObject::get_engine(): engine is null.");

	return *_runtime;
}

void minty::SceneObject::set_runtime(Runtime& engine)
{
	_runtime = &engine;
}

Scene& minty::SceneObject::get_scene() const
{
	return get_runtime().get_scene_manager().get_scene(_sceneId);
}

ID minty::SceneObject::get_scene_id() const
{
	return _sceneId;
}

void minty::SceneObject::set_scene(ID const sceneId)
{
	_sceneId = sceneId;
}
