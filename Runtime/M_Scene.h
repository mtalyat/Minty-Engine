#pragma once
#include "M_Object.h"

#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

namespace minty
{
	/// <summary>
	/// 
	/// </summary>
	class Scene :
		public Object
	{
	private:
		EntityRegistry _entities;
		SystemRegistry _systems;

	public:
		/// <summary>
		/// Creates an empty Scene.
		/// </summary>
		Scene();

		/// <summary>
		/// Gets the EntityRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		EntityRegistry& get_entity_registry();

		/// <summary>
		/// Gets the SystemRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		SystemRegistry& get_system_registry();

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
	};
}