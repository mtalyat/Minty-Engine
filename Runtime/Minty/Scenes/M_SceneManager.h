#pragma once
#include "Minty/Types/M_Object.h"
#include "Minty/Types/M_Time.h"

#include <unordered_map>

namespace Minty
{
	class Runtime;
	class Scene;

	class SceneManager
		: public Object
	{
	private:
		// if the scene manager has been loaded or not
		// if not, no scenes will receive events
		bool _loaded;
		// scene that is active, eg. receiving update, etc. events
		Ref<Scene> _loadedScene;
		Ref<Scene> _workingScene;
	public:
		SceneManager() = default;
		~SceneManager() = default;

		/// <summary>
		/// Checks if this SceneManager is loaded or not.
		/// </summary>
		/// <returns></returns>
		bool is_loaded() const { return _loaded; }

		/// <summary>
		/// Checks if this SceneManager is loaded, and if there is a scene loaded.
		/// </summary>
		/// <returns></returns>
		bool is_scene_loaded() const { return _loaded && _loadedScene.get(); }

		/// <summary>
		/// Creates and loads an empty Scene.
		/// </summary>
		/// <param name="path">The path to the .scene file.</param>
		/// <returns>The ID of the new Scene.</returns>
		Ref<Scene> create_scene(Path const& path);

		/// <summary>
		/// Destroys the Scene with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		bool destroy_scene(UUID const id);

		/// <summary>
		/// Checks if the Scene with the given ID has been created.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		bool is_created(UUID const id) const;

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
		Ref<Scene> get_loaded_scene() const;

		/// <summary>
		/// Gets the working Scene.
		/// 
		/// The working Scene is the scene that is actively being created, destroyed, or loaded.
		/// This is not the same as the loaded Scene, although it can be.
		/// </summary>
		/// <returns></returns>
		Ref<Scene> get_working_scene() const;

		/// <summary>
		/// Gets the Scene with the given id.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A pointer to the Scene, or nullptr if none found.</returns>
		Ref<Scene> get_scene(UUID const id);

		/// <summary>
		/// Gets the Scene with the given id.
		/// </summary>
		/// <param name="id">The ID of the Scene.</param>
		/// <returns>A reference to the Scene.</returns>
		Ref<Scene> at_scene(UUID const id);

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
		void update(Time const time);

		/// <summary>
		/// Fixed updates the active Scenes.
		/// </summary>
		void fixed_update(Time const time);

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
		void set_loaded_scene(Ref<Scene> const scene);

		void set_working_scene(Ref<Scene> const scene);
	};
}