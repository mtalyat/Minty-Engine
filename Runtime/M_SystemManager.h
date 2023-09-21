#pragma once

#include "M_Object.h"
#include "M_System.h"
#include <map>
#include <set>

namespace minty
{
	/// <summary>
	/// Holds and managers data relevant to systems. Can hold multiple types of systems.
	/// </summary>
	class SystemManager
		: public Object
	{
	private:
		// the systems to manage
		std::map<int, std::set<System*>> _systems;

	public:
		/// <summary>
		/// Creates an empty SystemManager.
		/// </summary>
		SystemManager();

		~SystemManager();

		/// <summary>
		/// Places the given System within the SystemManager.
		/// </summary>
		/// <param name="system">The system to add.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		void emplace(System* const system, int const priority = 0);

		/// <summary>
		/// Removes the given System from the SystemManager.
		/// </summary>
		/// <param name="system"></param>
		void erase(System* const system);

		/// <summary>
		/// Runs the load method on each System within this SystemManager.
		/// </summary>
		void load();

		/// <summary>
		/// Runs the update method on each System within this SystemManager.
		/// </summary>
		void update();

		/// <summary>
		/// Runs the fixedUpdate method on each System within this SystemManager.
		/// </summary>
		void fixedUpdate();

		/// <summary>
		/// Runs the unload method on each System within this SystemManager.
		/// </summary>
		void unload();
	};
}