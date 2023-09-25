#pragma once

#include "M_Object.h"
#include "M_System.h"
#include "M_Console.h"
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
		typedef std::function<System* (Engine* const, EntityRegistry* const)> SystemFunc;

	private:
		Engine* _engine;

		EntityRegistry* _registry;

		// the systems to manage
		std::map<int, std::set<System*>>* _systems;

		static std::map<std::string const, SystemFunc const> _systemTypes;
	public:
		/// <summary>
		/// Creates an empty SystemRegistry.
		/// </summary>
		SystemRegistry(Engine* const engine, EntityRegistry* const registry);

		~SystemRegistry();

		// move
		SystemRegistry(SystemRegistry&& other) noexcept;

		SystemRegistry& operator=(SystemRegistry&& other) noexcept;

	private:
		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <param name="system">The system to add.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		void emplace(System* const system, int const priority = 0);

	public:
		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <typeparam name="T">The system to add.</typeparam>
		/// <param name="priority">The priority in which to update this System in.</param>
		template<class T>
		void emplace(int const priority = 0);

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
		/// Registers the System, so the System can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The System to be registered.</typeparam>
		template <class T>
		static void register_system(std::string const& name);

		std::string const to_string() const override;
	};

	template<class T>
	inline void SystemRegistry::emplace(int const priority)
	{
		this->emplace(new T(_engine, _registry), priority);
	}

	template<class T>
	void SystemRegistry::register_system(std::string const& name)
	{
		_systemTypes.emplace(name, [](Engine* const engine, EntityRegistry* const registry) { return new T(engine, registry); });

		console::info(std::format("Registered system {}", name));
	}
}