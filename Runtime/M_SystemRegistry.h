#pragma once

#include "M_Object.h"
#include "M_System.h"
#include "M_Console.h"
#include "M_ISerializable.h"
#include <map>
#include <set>

namespace minty
{
	/// <summary>
	/// Holds and managers data relevant to systems.
	/// </summary>
	class SystemRegistry
		: public Object, public ISerializable
	{
	public:
		typedef std::function<System* (Engine* const, EntityRegistry* const)> SystemFunc;

	private:
		Engine* _engine;

		EntityRegistry* _registry;

		// the systems to manage
		std::map<int, std::set<System*>> _orderedSystems;
		std::map<std::string const, System*> _allSystems;

		static std::map<std::string const, SystemFunc const> _systemTypes;
	public:
		/// <summary>
		/// Creates an empty SystemRegistry.
		/// </summary>
		SystemRegistry(Engine* const engine, EntityRegistry* const registry);

		~SystemRegistry();

		// move
		SystemRegistry(SystemRegistry&& other) noexcept;

		// move
		SystemRegistry& operator=(SystemRegistry&& other) noexcept;

	private:
		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <param name="system">The system to add.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		System* emplace(std::string const& name, System* const system, int const priority = 0);

	public:
		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <typeparam name="T">The system to add.</typeparam>
		/// <param name="priority">The priority in which to update this System in.</param>
		template<class T>
		T* emplace(std::string const& name, int const priority = 0);

		/// <summary>
		/// Creates and places a registered System within this SystemRegistry.
		/// </summary>
		/// <param name="name">The name of the System, registered with register_system.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		System* emplace_by_name(std::string const& name, int const priority = 0);

		/// <summary>
		/// Finds the System that matches the given name.
		/// </summary>
		/// <param name="name">The name to search by.</param>
		/// <returns>The System if found, otherwise null.</returns>
		System* find_by_name(std::string const& name) const;

		/// <summary>
		/// Finds the first System of the given type.
		/// </summary>
		/// <typeparam name="T">The type of System to return.</typeparam>
		/// <returns>The first System found, or null if none found.</returns>
		template<class T>
		T* find_by_type() const;

		/// <summary>
		/// Removes the given System from the SystemRegistry.
		/// </summary>
		/// <param name="system"></param>
		void erase(System* const system);

		/// <summary>
		/// Gets the number of systems within the registry.
		/// </summary>
		/// <returns>The total number of systems within the registry.</returns>
		size_t size() const;

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
		/// Clears the entire registry.
		/// </summary>
		void clear();

		/// <summary>
		/// Registers the System, so the System can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The System to be registered.</typeparam>
		template <class T>
		static void register_system(std::string const& name);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	public:
		friend std::string to_string(SystemRegistry const& value);
};

	template<class T>
	T* SystemRegistry::emplace(std::string const& name, int const priority)
	{
		return static_cast<T*>(this->emplace(name, new T(_engine, _registry), priority));
	}

	template<class T>
	inline T* SystemRegistry::find_by_type() const
	{
		for (auto const& pair : _allSystems)
		{
			if (typeid(*pair.second) == typeid(T))
			{
				// found, type matches
				return static_cast<T*>(pair.second);
			}
		}

		// not found
		return nullptr;
	}

	template<class T>
	void SystemRegistry::register_system(std::string const& name)
	{
		_systemTypes.emplace(name, [](Engine* const engine, EntityRegistry* const registry) { return new T(engine, registry); });

		console::info(std::format("Registered system {}", name));
	}
}