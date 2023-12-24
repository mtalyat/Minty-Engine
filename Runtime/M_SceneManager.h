#pragma once

#include "M_Object.h"
#include "M_Scene.h"
#include <vector>
#include <unordered_set>

namespace minty
{
	class Engine;

	class SceneManager
		: public Object
	{
	private:
		Engine* _engine;

		// if the scene manager has been loaded or not
		// if not, no scenes will receive events
		bool _loaded;
		// all scenes stored in memory
		std::vector<Scene> _scenes;
		// scene that is active, eg. receiving update, etc. events
		Scene* _loadedScene;

	public:
		SceneManager(Engine* const engine);

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
		ID create_scene(String const& path);

		/// <summary>
		/// Loads the Scene with the given ID. Unloads the current Scene first.
		/// </summary>
		/// <param name="id">The ID of the Scene to load.</param>
		/// <param name="camera">The Entity with the CameraComponent to render to.</param>
		void load_scene(ID const id);

		/// <summary>
		/// Gets the loaded Scene.
		/// </summary>
		/// <returns>A pointer to the active scene, or null if none loaded.</returns>
		Scene* get_loaded_scene();

		/// <summary>
		/// Get the Scene based on its ID.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A reference to the Scene.</returns>
		Scene& get_scene(ID const id);

		/// <summary>
		/// Gets the number of Scenes in this SceneManager.
		/// </summary>
		/// <returns>The number of Scenes.</returns>
		size_t size() const;

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

		/// <summary>
		/// Finalizes updating the active Scene.
		/// </summary>
		void finalize();

	public:
		friend String to_string(SceneManager const& value);
	};
}