#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Time.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	class Runtime;
	class Scene;

	class SceneManager
	{
	private:
		// scene that is active, eg. receiving update, etc. events
		Ref<Scene> m_loadedScene;
		std::vector<Ref<Scene>> m_workingScenes;
	public:
		SceneManager()
			: m_loadedScene(nullptr)
			, m_workingScenes()
		{}
		~SceneManager() = default;

		/// <summary>
		/// Loads a new Scene.
		/// </summary>
		/// <param name="scene"></param>
		void load(Path const& path);

		/// <summary>
		/// Unloads the currently loaded Scene.
		/// </summary>
		void unload();

		/// <summary>
		/// Updates the active Scene.
		/// </summary>
		void update(Time const time);

		/// <summary>
		/// Fixed updates the active Scene.
		/// </summary>
		void fixed_update(Time const time);

		/// <summary>
		/// Finalizes updating the active Scene.
		/// </summary>
		void finalize();

		Ref<Scene> get_working_scene() const;

		void push_working_scene(Ref<Scene> const scene);

		void pop_working_scene();

	private:
		void set_loaded_scene(Ref<Scene> const scene);

	public:
		Ref<Scene> get_loaded_scene() const { return m_loadedScene; }
	};
}