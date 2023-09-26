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
	, _activeScenes()
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

void minty::SceneManager::activate_scene(ID const id)
{
	Scene* scene = &_scenes.at(id);

	// do nothing if already loaded
	if (_activeScenes.contains(scene))
	{
		return;
	}

	// trigger activate event, if manager is loaded
	if (_loaded)
	{
		scene->load();
	}

	// add to active list
	_activeScenes.emplace(scene);
}

void minty::SceneManager::activate_scene_exclusive(ID const id)
{
	// if already loaded, just unload others
	// if not loaded, unload all, then load this one

	Scene* scene = &_scenes.at(id);

	if (_activeScenes.contains(scene))
	{
		// unload all scenes except the one we want
		for (auto const& s : _activeScenes)
		{
			// skip the one we want loaded
			if (scene == s) continue;

			// unload others
			if (_loaded)
			{
				s->unload();
			}

			console::error("Attempting to unload scene when SceneManager is not in loaded state.");
		}
		
		// clear list
		_activeScenes.clear();

		// add target
		_activeScenes.emplace(scene);
	}
	else
	{
		// unload all
		deactivate_all();

		// load target
		activate_scene(id);
	}
}

void minty::SceneManager::deactivate_scene(ID const id)
{
	Scene* scene = &_scenes.at(id);

	// do nothing if already unloaded
	if (!_activeScenes.contains(scene))
	{
		return;
	}

	// trigger deactivate event if manager is loaded
	if (_loaded)
	{
		scene->unload();
	}
	else
	{
		console::error("Unloading scene when SceneManager is not in loaded state.");
	}

	// remove from active list
	_activeScenes.erase(scene);
}

void minty::SceneManager::deactivate_all()
{
	if (_loaded)
	{
		// unload all scenes
		for (auto const& s : _activeScenes)
		{
			s->unload();
		}
	}
	else
	{
		console::error("Unloading all scenes when SceneManager is not in loaded state.");
	}

	// clear active scene list
	_activeScenes.clear();
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
