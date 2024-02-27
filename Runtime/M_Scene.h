#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;
	class EntityRegistry;
	class SystemRegistry;

	/// <summary>
	/// Holds a collection of systems and entities that can interact with one another.
	/// </summary>
	class Scene :
		public Object
	{
	private:
		ID _id;
		Runtime* _engine;
		EntityRegistry* _entities;
		SystemRegistry* _systems;
		bool _loaded;

	public:
		/// <summary>
		/// Creates an empty Scene.
		/// </summary>
		Scene(Runtime& engine, ID const sceneId);

		~Scene();

		// move
		Scene(Scene&& other) noexcept;

		// move
		Scene& operator=(Scene&& other) noexcept;

		//// copy
		//Scene(Scene const& other);

		//// copy
		//Scene& operator=(Scene const& other);

		Runtime& get_runtime() const;

		/// <summary>
		/// Gets the EntityRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		EntityRegistry& get_entity_registry() const;

		/// <summary>
		/// Gets the SystemRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		SystemRegistry& get_system_registry() const;

		/// <summary>
		/// Checks if this Scene is loaded or not.
		/// </summary>
		/// <returns></returns>
		bool is_loaded() const;

		/// <summary>
		/// Loads this Scene.
		/// </summary>
		void load();

		/// <summary>
		/// Updates this Scene.
		/// </summary>
		void update();

		/// <summary>
		/// Sorts the Entities in this Scene based on hierarchy.
		/// </summary>
		void sort();

		/// <summary>
		/// Fixed updates this Scene.
		/// </summary>
		void fixed_update();

		/// <summary>
		/// Unloads this Scene.
		/// </summary>
		void unload();

		/// <summary>
		/// Finalized updating this Scene.
		/// </summary>
		void finalize();

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	public:
		friend String to_string(Scene const& value);
	};
}