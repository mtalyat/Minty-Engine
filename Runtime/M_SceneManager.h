#pragma once

#include "M_Object.h"
#include "M_Scene.h"
#include <vector>
#include <unordered_set>

namespace minty
{
	class Runtime;

	class SceneManager
		: public Object
	{
	private:
		// if the scene manager has been loaded or not
		// if not, no scenes will receive events
		bool _loaded;
		// all scenes stored in memory
		std::vector<Scene> _scenes;
		// scenes that are active, eg. receiving update, etc. events
		std::unordered_set<Scene*> _activeScenes;

	public:
		SceneManager();

		/// <summary>
		/// Creates and loads an empty Scene.
		/// </summary>
		/// <returns>The ID of the new Scene.</returns>
		ID create_scene();

		/// <summary>
		/// Loads a new Scene using the file data at the given path.
		/// </summary>
		/// <param name="path">The path to the .scene file.</param>
		/// <returns>The ID of the new Scene.</returns>
		ID create_scene(std::string const& path);

		/// <summary>
		/// Loads the Scene with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Scene to load.</param>
		void activate_scene(ID const id);

		/// <summary>
		/// Loads the Scene with the given ID, and unloads all other loaded Scenes.
		/// </summary>
		/// <param name="id">The ID of the Scene to load.</param>
		void activate_scene_exclusive(ID const id);

		/// <summary>
		/// Unloads the Scene with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Scene to unload.</param>
		void deactivate_scene(ID const id);

		/// <summary>
		/// Unloads all loaded Scenes.
		/// </summary>
		void deactivate_all();

		/// <summary>
		/// Get the Scene based on its ID.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A reference to the Scene.</returns>
		Scene& get_scene(ID const id);

		/// <summary>
		/// Loads the SceneManager and any active Scenes.
		/// </summary>
		void load();

		/// <summary>
		/// Updates the active Scenes.
		/// </summary>
		void update();

		/// <summary>
		/// Fixed updates the active Scenes.
		/// </summary>
		void fixed_update();

		/// <summary>
		/// Unloads the SceneManager and any active Scenes.
		/// </summary>
		void unload();
	};
}