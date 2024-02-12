#include "pch.h"
#include "M_SceneObject.h"

#include "M_Runtime.h"
#include "M_Scene.h"

using namespace minty;

minty::SceneObject::SceneObject()
	: _engine()
	, _sceneId()
{}

minty::SceneObject::SceneObject(Runtime& engine, ID const sceneId)
	: _engine(&engine)
	, _sceneId(sceneId)
{}

minty::SceneObject::~SceneObject()
{
}

Runtime& minty::SceneObject::get_engine() const
{
	MINTY_ASSERT(_engine != nullptr, "SceneObject::get_engine(): engine is null.");

	return *_engine;
}

void minty::SceneObject::set_engine(Runtime& engine)
{
	_engine = &engine;
}

Scene& minty::SceneObject::get_scene() const
{
	return get_engine().get_scene_manager().get_scene(_sceneId);
}

ID minty::SceneObject::get_scene_id() const
{
	return _sceneId;
}

void minty::SceneObject::set_scene(ID const sceneId)
{
	_sceneId = sceneId;
}
