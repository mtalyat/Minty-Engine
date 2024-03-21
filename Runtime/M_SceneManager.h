#pragma once
#include "M_RuntimeObject.h"

#include <unordered_map>

namespace minty
{
	class Runtime;
	class Scene;

	class SceneManager
		: public RuntimeObject
	{
	private:
		// if the scene manager has been loaded or not
		// if not, no scenes will receive events
		bool _loaded;
		// scene that is active, eg. receiving update, etc. events
		Scene* _loadedScene;
		Scene* _workingScene;
	public:
		SceneManager(Runtime& engine);

		/// <summary>
		/// Checks if this SceneManager is loaded or not.
		/// </summary>
		/// <returns></returns>
		bool is_loaded() const;

		/// <summary>
		/// Creates and loads an empty Scene.
		/// </summary>
		/// <param name="path">The path to the .scene file.</param>
		/// <returns>The ID of the new Scene.</returns>
		Scene& create_scene(Path const& path);

		/// <summary>
		/// Destroys the Scene with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		bool destroy_scene(UUID const id);

		/// <summary>
		/// Loads the Scene with the given ID. Unloads the current Scene first.
		/// </summary>
		/// <param name="id">The ID of the Scene to load.</param>
		/// <param name="camera">The Entity with the CameraComponent to render to.</param>
		void load_scene(UUID const id);

		/// <summary>
		/// Unloads the currently loaded Scene.
		/// </summary>
		void unload_scene();

		/// <summary>
		/// Gets the loaded Scene.
		/// </summary>
		/// <returns>A pointer to the active scene, or null if none loaded.</returns>
		Scene* get_loaded_scene() const;

		/// <summary>
		/// Gets the working Scene.
		/// 
		/// The working Scene is the scene that is actively being created, destroyed, or loaded.
		/// This is not the same as the loaded Scene, although it can be.
		/// </summary>
		/// <returns></returns>
		Scene* get_working_scene() const;

		/// <summary>
		/// Gets the Scene with the given id.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A pointer to the Scene, or nullptr if none found.</returns>
		Scene* get_scene(UUID const id);

		/// <summary>
		/// Gets the Scene with the given id.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A reference to the Scene.</returns>
		Scene& at_scene(UUID const id);

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

		/// <summary>
		/// Destroys the SceneManager and all Scenes.
		/// </summary>
		void destroy();

	private:
		void set_loaded_scene(Scene* const scene);

		void set_working_scene(Scene* const scene);
	public:
		friend String to_string(SceneManager const& value);
	};
}