#pragma once
#include "M_Object.h"

#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_ISerializable.h"

namespace minty
{
	class Engine;

	/// <summary>
	/// Holds a collection of systems and entities that can interact with one another.
	/// </summary>
	class Scene :
		public Object, public ISerializable
	{
	private:
		Engine* _engine;
		EntityRegistry* _entities;
		SystemRegistry* _systems;

	public:
		/// <summary>
		/// Creates an empty Scene.
		/// </summary>
		Scene(Engine* const engine);

		~Scene();

		// move
		Scene(Scene&& other) noexcept;

		// move
		Scene& operator=(Scene&& other) noexcept;

		Engine* get_engine() const;

		/// <summary>
		/// Gets the EntityRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		EntityRegistry* get_entity_registry() const;

		/// <summary>
		/// Gets the SystemRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		SystemRegistry* get_system_registry() const;

		/// <summary>
		/// Loads this Scene.
		/// </summary>
		void load();

		/// <summary>
		/// Updates this Scene.
		/// </summary>
		void update();

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