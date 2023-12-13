#include "pch.h"
#include "M_SceneManager.h"

#include "M_Assets.h"
#include "M_Runtime.h"
#include "M_Engine.h"
#include "M_Node.h"
#include "M_Reader.h"
#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_Engine.h"
#include "M_Renderer.h"

#include "M_NameComponent.h"

using namespace minty;

minty::SceneManager::SceneManager(Engine* const engine)
	: _engine(engine)
	, _loaded()
	, _scenes()
	, _loadedScene()
{}

ID minty::SceneManager::create_scene()
{
	ID id = static_cast<ID>(_scenes.size());
	_scenes.push_back(Scene(_engine));
	return id;
}

ID minty::SceneManager::create_scene(std::string const& path)
{
	// create the scene
	ID id = create_scene();
	Scene& scene = _scenes.at(id);

	// load the data from the disk into the scene
	Node node = assets::load_node(path);
	SerializationData data =
	{
		.scene = &scene,
		.entity = NULL_ENTITY
	};
	Reader reader(node, &data);
	scene.deserialize(reader);

	// all done
	return id;
}

void minty::SceneManager::load_scene(ID const id)
{
	Scene* scene = &_scenes.at(id);

	// do nothing if already loaded
	if (scene == _loadedScene)
	{
		return;
	}

	// if scene already loaded, unload it
	if (_loaded && _loadedScene)
	{
		_loadedScene->unload();
	}

	// set value
	_loadedScene = scene;

	// set renderer to use this new scene
	_engine->get_renderer()->set_scene(_loadedScene);
	_engine->get_audio_engine()->set_scene(_loadedScene);

	// load event
	if (_loaded && _loadedScene)
	{
		_loadedScene->load();
	}
}

Scene* minty::SceneManager::get_loaded_scene()
{
	return _loadedScene;
}

Scene& minty::SceneManager::get_scene(ID const id)
{
	return _scenes.at(id);
}

size_t minty::SceneManager::size() const
{
	return _scenes.size();
}

void minty::SceneManager::load()
{
	// do nothing if already loaded
	if (_loaded)
	{
		return;
	}

	// mark as loaded
	_loaded = true;

	// load active scene
	if (_loadedScene)
	{
		_loadedScene->load();
	}
}

void minty::SceneManager::update()
{
	// update all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->update();
	}
}

void minty::SceneManager::fixed_update()
{
	// fixed update all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->fixed_update();
	}
}

void minty::SceneManager::unload()
{
	// do nothing if already unloaded
	if (!_loaded)
	{
		return;
	}

	// mark as unloaded
	_loaded = false;

	if (_loadedScene)
	{
		_loadedScene->unload();
	}
}

void minty::SceneManager::finalize()
{
	// finalize all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->finalize();
	}
}

std::string minty::to_string(SceneManager const& value)
{
	return std::format("SceneManager(scenes size = {})", value.size());
}
