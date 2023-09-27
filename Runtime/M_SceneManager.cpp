#include "pch.h"
#include "M_SceneManager.h"

#include "M_Runtime.h"
#include "M_Engine.h"
#include "M_SerializedNode.h"
#include "M_Reader.h"
#include "M_Console.h"

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
	SerializedNode node = SerializedNode::parse_file(path);
	Reader reader(node);
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

void minty::SceneManager::load()
{
	// do nothing if already loaded
	if (_loaded)
	{
		return;
	}

	// mark as loaded
	_loaded = true;

	// load all active scenes
	for (auto const scene : _activeScenes)
	{
		scene->load();
	}
}

void minty::SceneManager::update()
{
	// update all active scenes if loaded
	if (_loaded)
	{
		for (auto const scene : _activeScenes)
		{
			scene->update();
		}
	}
}

void minty::SceneManager::fixed_update()
{
	// fixed update all active scenes if loaded
	if (_loaded)
	{
		for (auto const scene : _activeScenes)
		{
			scene->fixed_update();
		}
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

	// unload all active scenes
	for (auto const scene : _activeScenes)
	{
		scene->unload();
	}
}
