#include "pch.h"
#include "Minty/Scenes/M_SceneManager.h"

#include "Minty/Assets/M_Asset.h"
#include "Minty/Types/M_Node.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Assets/M_AssetEngine.h"

#include "Minty/Core/M_Application.h"
#include "Minty/Layers/M_DefaultLayer.h"

#include "Minty/Components/M_NameComponent.h"

using namespace Minty;

Scene& Minty::SceneManager::create_scene(Path const& path)
{
	// load the data from the disk
	AssetEngine& assets = AssetEngine::instance();
	Node node = assets.read_file_node(path);
	Node meta = assets.read_file_meta(path);

	SceneBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path
	};

	Ref<Scene> scene = create_ref<Scene>(builder);
	Reader reader(node);
	scene->deserialize(reader);

	// add to assets
	assets.emplace(scene);

	// all done
	return *scene;
}

bool Minty::SceneManager::destroy_scene(UUID const id)
{
	AssetEngine& assets = AssetEngine::instance();

	if (assets.contains(id))
	{
		// set active scene temporarily
		Ref<Scene> scene = assets.at<Scene>(id);

		set_working_scene(scene);

		// if this is the loaded scene, unload it first
		if (_loadedScene == scene)
		{
			unload_scene();
		}

		// remove from assets
		AssetEngine& assets = AssetEngine::instance();
		assets.unload(id);

		set_working_scene(_loadedScene);

		return true;
	}

	return false;
}

void Minty::SceneManager::load_scene(UUID const id)
{
	// ID must be zero (null) or scenes must contain the id
	AssetEngine& assets = AssetEngine::instance();
	MINTY_ASSERT(assets.contains(id));

	Ref<Scene> scene = get_scene(id);

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

void Minty::SceneManager::unload_scene()
{
	if (_loadedScene)
	{
		_loadedScene->unload();
		set_loaded_scene(nullptr);
	}
}

Ref<Scene> Minty::SceneManager::get_loaded_scene() const
{
	return _loadedScene;
}

Ref<Scene> Minty::SceneManager::get_working_scene() const
{
	return _workingScene;
}

Ref<Scene> Minty::SceneManager::get_scene(UUID const id)
{
	AssetEngine& assets = AssetEngine::instance();
	return assets.get<Scene>(id);
}

Ref<Scene> Minty::SceneManager::at_scene(UUID const id)
{
	AssetEngine& assets = AssetEngine::instance();
	return assets.at<Scene>(id);
}

size_t Minty::SceneManager::size() const
{
	AssetEngine& assets = AssetEngine::instance();
	return assets.get_by_type<Scene>().size();
}

void Minty::SceneManager::load()
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

void Minty::SceneManager::update(Time const time)
{
	// update all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->update(time);
	}
}

void Minty::SceneManager::fixed_update(Time const time)
{
	// fixed update all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->fixed_update(time);
	}
}

void Minty::SceneManager::unload()
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

void Minty::SceneManager::finalize()
{
	// finalize all active scenes if loaded
	if (_loaded && _loadedScene)
	{
		_loadedScene->finalize();
	}
}

void Minty::SceneManager::destroy()
{
	unload();

	AssetEngine& assets = AssetEngine::instance();
	for (auto const scene : assets.get_by_type<Scene>())
	{
		destroy_scene(scene->get_id());
	}
	set_working_scene(nullptr);
	set_loaded_scene(nullptr);
}

void Minty::SceneManager::set_loaded_scene(Ref<Scene> const scene)
{
	_loadedScene = scene;
	Application::instance().set_loaded_scene(scene);
}

void Minty::SceneManager::set_working_scene(Ref<Scene> const scene)
{
	_workingScene = scene;
	Application::instance().set_working_scene(scene);
}