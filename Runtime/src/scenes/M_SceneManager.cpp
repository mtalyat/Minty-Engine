#include "pch.h"
#include "scenes/M_SceneManager.h"

#include "assets/M_Asset.h"
#include "runtime/M_Runtime.h"
#include "runtime/M_Runtime.h"
#include "types/M_Node.h"
#include "serialization/M_Reader.h"
#include "tools/M_Console.h"
#include "serialization/M_SerializationData.h"
#include "scenes/M_Scene.h"
#include "runtime/M_Runtime.h"
#include "rendering/M_RenderEngine.h"
#include "assets/M_AssetEngine.h"

#include "components/M_NameComponent.h"

using namespace minty;

minty::SceneManager::SceneManager(Runtime& engine)
	: RuntimeObject(engine)
	, _loaded()
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
	AssetEngine& assets = get_runtime().get_asset_engine();
	Node node = assets.read_file_node(path);
	Node meta = assets.read_file_meta(path);

	SceneBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path
	};

	Scene* scene = new Scene(builder, get_runtime());
	Reader reader(node);
	scene->deserialize(reader);

	// add to assets
	assets.emplace(scene);

	// all done
	return *scene;
}

bool minty::SceneManager::destroy_scene(UUID const id)
{
	AssetEngine& assets = get_runtime().get_asset_engine();

	if (assets.contains(id))
	{
		// set active scene temporarily
		Scene& scene = assets.at<Scene>(id);

		set_working_scene(&scene);

		// if this is the loaded scene, unload it first
		if (_loadedScene == &scene)
		{
			unload_scene();
		}

		// remove from assets
		AssetEngine& assets = get_runtime().get_asset_engine();
		assets.unload(id);

		set_working_scene(_loadedScene);

		return true;
	}

	return false;
}

void minty::SceneManager::load_scene(UUID const id)
{
	// ID must be zero (null) or scenes must contain the id
	AssetEngine& assets = get_runtime().get_asset_engine();
	MINTY_ASSERT(assets.contains(id));

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
	AssetEngine& assets = get_runtime().get_asset_engine();
	return assets.get<Scene>(id);
}

Scene& minty::SceneManager::at_scene(UUID const id)
{
	AssetEngine& assets = get_runtime().get_asset_engine();
	MINTY_ASSERT(assets.contains(id));
	return assets.at<Scene>(id);
}

size_t minty::SceneManager::size() const
{
	AssetEngine& assets = get_runtime().get_asset_engine();
	return assets.get_by_type<Scene>().size();
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
	unload();

	AssetEngine& assets = get_runtime().get_asset_engine();
	for (auto const scene : assets.get_by_type<Scene>())
	{
		destroy_scene(scene->get_id());
	}
	set_working_scene(nullptr);
	set_loaded_scene(nullptr);
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
