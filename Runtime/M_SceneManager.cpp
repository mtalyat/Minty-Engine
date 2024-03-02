#include "pch.h"
#include "M_SceneManager.h"

#include "M_Asset.h"
#include "M_Runtime.h"
#include "M_Runtime.h"
#include "M_Node.h"
#include "M_Reader.h"
#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_Runtime.h"
#include "M_RenderEngine.h"
#include "M_AssetEngine.h"

#include "M_NameComponent.h"

using namespace minty;

minty::SceneManager::SceneManager(Runtime& engine)
	: RuntimeObject(engine)
	, _loaded()
	, _scenes()
	, _loadedScene()
	, _workingScene()
{}

bool minty::SceneManager::is_loaded() const
{
	return _loaded;
}

Scene& minty::SceneManager::create_scene(Path const& path)
{
	// load the data from the disk
	Node node = Asset::load_node(path);

	SceneBuilder builder
	{
		.id = node.to_uuid(),
		.path = path
	};

	Scene* scene = new Scene(builder, get_runtime());
	_scenes.emplace(scene->get_id(), scene);

	AssetEngine& assets = get_runtime().get_asset_engine();
	assets.emplace(scene);

	// set to working scene for creation
	set_working_scene(scene);

	// deserialize
	SerializationData data =
	{
		.scene = scene,
		.entity = NULL_ENTITY
	};
	Reader reader(node, &data);
	scene->deserialize(reader);

	// all done, set working scene back to the loaded scene
	set_working_scene(_loadedScene);

	// all done
	return *scene;
}

bool minty::SceneManager::destroy_scene(UUID const id)
{
	if (_scenes.contains(id))
	{
		// set active scene temporarily
		Scene* scene = _scenes.at(id);

		set_working_scene(scene);

		// if this is the loaded scene, unload it first
		if (_loadedScene == scene)
		{
			unload_scene();
		}

		// delete
		delete _scenes.at(id);
		// erase
		_scenes.erase(id);

		set_working_scene(_loadedScene);

		return true;
	}

	return false;
}

void minty::SceneManager::load_scene(UUID const id)
{
	// ID must be zero (null) or scenes must contain the id
	MINTY_ASSERT(!id.valid() || _scenes.contains(id));

	Scene* scene = get_scene(id);

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

	// set scene values
	set_loaded_scene(scene);
	set_working_scene(scene);

	// load event
	if (_loaded && _loadedScene)
	{
		_loadedScene->load();
	}
}

void minty::SceneManager::unload_scene()
{
	if (_loadedScene)
	{
		_loadedScene->unload();
		set_loaded_scene(nullptr);
	}
}

Scene* minty::SceneManager::get_loaded_scene() const
{
	return _loadedScene;
}

Scene* minty::SceneManager::get_working_scene() const
{
	return _workingScene;
}

Scene* minty::SceneManager::get_scene(UUID const id)
{
	auto found = _scenes.find(id);

	if (found != _scenes.end())
	{
		return found->second;
	}

	return nullptr;
}

Scene& minty::SceneManager::at_scene(UUID const id)
{
	MINTY_ASSERT(_scenes.contains(id));

	return *_scenes.at(id);
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

void minty::SceneManager::destroy()
{
	for (auto const& pair : _scenes)
	{
		delete pair.second;
	}
	_scenes.clear();
}

void minty::SceneManager::set_loaded_scene(Scene* const scene)
{
	_loadedScene = scene;
	get_runtime().set_loaded_scene(scene);
}

void minty::SceneManager::set_working_scene(Scene* const scene)
{
	_workingScene = scene;
	get_runtime().set_working_scene(scene);
}

String minty::to_string(SceneManager const& value)
{
	return std::format("SceneManager(scenes size = {})", value.size());
}
