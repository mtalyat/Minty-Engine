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
	class SystemRegistry
		: public Object
	{
	public:
		typedef std::function<System* ()> SystemFunc;

	private:
		static std::map<std::string const, SystemFunc const> _systemTypes;



		// the systems to manage
		std::map<int, std::set<System*>> _systems;

	public:
		/// <summary>
		/// Creates an empty SystemRegistry.
		/// </summary>
		SystemRegistry();

		~SystemRegistry();

		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <param name="system">The system to add.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		void emplace(System* const system, int const priority = 0);

		/// <summary>
		/// Creates and places a registered System within this SystemRegistry.
		/// </summary>
		/// <param name="name">The name of the System, registered with register_system.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		void emplace_by_name(std::string const& name, int const priority = 0);

		/// <summary>
		/// Removes the given System from the SystemRegistry.
		/// </summary>
		/// <param name="system"></param>
		void erase(System* const system);

		/// <summary>
		/// Runs the load method on each System within this SystemRegistry.
		/// </summary>
		void load();

		/// <summary>
		/// Runs the update method on each System within this SystemRegistry.
		/// </summary>
		void update();

		/// <summary>
		/// Runs the fixedUpdate method on each System within this SystemRegistry.
		/// </summary>
		void fixed_update();

		/// <summary>
		/// Runs the unload method on each System within this SystemRegistry.
		/// </summary>
		void unload();

		/// <summary>
		/// Registers the System creation function, so the System can be dynamically created by name.
		/// </summary>
		/// <param name="name">The name of the System class.</param>
		/// <param name="func">A func that creates a new System and returns it.</param>
		static void register_system(std::string const& name, SystemFunc const& func);
	};
}