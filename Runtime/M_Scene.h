#pragma once
#include "M_Object.h"

#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_ISerializable.h"

namespace minty
{
	class Engine;

	/// <summary>
	/// 
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

		Scene(Scene&& other) noexcept;

		Scene& operator=(Scene&& other) noexcept;

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
		/// Loads the Scene.
		/// </summary>
		void load();

		/// <summary>
		/// Updates the Scene.
		/// </summary>
		void update();

		/// <summary>
		/// Fixed updates the Scene.
		/// </summary>
		void fixed_update();

		/// <summary>
		/// Unloads the Scene.
		/// </summary>
		void unload();

		std::string const to_string() const override;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}