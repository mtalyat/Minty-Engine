#include "pch.h"
#include "SceneManager.h"

#include "Minty/Asset/Asset.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Component/NameComponent.h"
#include "Minty/Core/Application.h"
#include "Minty/Core/Console.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Serialize/Serializable.h"
#include "Minty/Render/Renderer.h"

using namespace Minty;

void Minty::SceneManager::load(Path const& path)
{
	// unload current
	unload();

	// load new
	m_loadedScene = AssetManager::load<Scene>(path);

	// set to working scene for easy access
	Application::instance().get_scene_manager().push_working_scene(m_loadedScene);

	m_loadedScene->load();
}

void Minty::SceneManager::unload()
{
	if (m_loadedScene == nullptr) return;

	m_loadedScene->unload();
	AssetManager::destroy(m_loadedScene->id());
	//AssetManager::unload(m_loadedScene->id());
	m_loadedScene = nullptr;
	
	// no longer working scene
	Application::instance().get_scene_manager().pop_working_scene();
}

void Minty::SceneManager::update(Time const time)
{
	// update all active scenes if loaded
	if (m_loadedScene != nullptr)
	{
		m_loadedScene->update(time);
	}
	else
	{
		MINTY_WARN("No scene loaded.");
	}
}

void Minty::SceneManager::fixed_update(Time const time)
{
	// fixed update all active scenes if loaded
	if (m_loadedScene != nullptr)
	{
		m_loadedScene->fixed_update(time);
	}
}

void Minty::SceneManager::finalize()
{
	// finalize all active scenes if loaded
	if (m_loadedScene != nullptr)
	{
		m_loadedScene->finalize();
	}
}

Ref<Scene> Minty::SceneManager::get_working_scene() const
{
	if (m_workingScenes.empty())
	{
		return Ref<Scene>();
	}

	return m_workingScenes.back();
}

void Minty::SceneManager::push_working_scene(Ref<Scene> const scene)
{
	m_workingScenes.push_back(scene);
}

void Minty::SceneManager::pop_working_scene()
{
	if (!m_workingScenes.empty())
	{
		m_workingScenes.pop_back();
	}
}

void Minty::SceneManager::set_loaded_scene(Ref<Scene> const scene)
{
	m_loadedScene = scene;
}