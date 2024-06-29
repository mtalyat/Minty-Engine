#pragma once
#include "Minty/Scenes/M_SceneObject.h"

#include "Minty/Types/M_Time.h"
#include <map>
#include <unordered_map>
#include <set>
#include <functional>
#include <vector>

namespace Minty
{
	class System;

	/// <summary>
	/// Holds and managers data relevant to systems.
	/// </summary>
	class SystemRegistry
		: public SceneObject
	{
		friend class Scene;
	public:
		typedef std::function<System* (Scene& scene)> SystemFunc;

	private:
		// the systems to manage
		std::map<int, std::set<System*>> _orderedSystems;
		std::map<String const, System*> _allSystems;
		std::unordered_map<TypeID, System*> _typeLookup;

		static std::map<String const, SystemFunc const> _systemTypes;
	public:
		/// <summary>
		/// Creates an empty SystemRegistry.
		/// </summary>
		SystemRegistry(Scene& scene);

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
		System* emplace(System* const system, int const priority = 0);

	public:
		/// <summary>
		/// Places the given System within this SystemRegistry.
		/// </summary>
		/// <typeparam name="T">The system to add.</typeparam>
		/// <param name="priority">The priority in which to update this System in.</param>
		template<class T>
		T* emplace(String const& name, int const priority = 0);

		/// <summary>
		/// Creates and places a registered System within this SystemRegistry.
		/// </summary>
		/// <param name="name">The name of the System, registered with register_system.</param>
		/// <param name="priority">The priority in which to update this System in.</param>
		System* emplace_by_name(String const& name, int const priority = 0);

		/// <summary>
		/// Finds the System that matches the given name.
		/// </summary>
		/// <param name="name">The name to search by.</param>
		/// <returns>The System if found, otherwise null.</returns>
		template<class T>
		T* find(String const& name) const;

		/// <summary>
		/// Finds the first System of the given type.
		/// </summary>
		/// <typeparam name="T">The type of System to return.</typeparam>
		/// <returns>The first System found, or null if none found.</returns>
		template<class T>
		T* find() const;

		/// <summary>
		/// Removes the System with the given name.
		/// </summary>
		/// <param name="name"></param>
		void erase_by_name(String const& name);

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
		void update(Time const time);

		/// <summary>
		/// Runs the finalize method on each System within this SystemRegistry.
		/// </summary>
		/// <param name="time"></param>
		void finalize();

		/// <summary>
		/// Runs the fixedUpdate method on each System within this SystemRegistry.
		/// </summary>
		void fixed_update(Time const time);

		/// <summary>
		/// Runs the unload method on each System within this SystemRegistry.
		/// </summary>
		void unload();

		/// <summary>
		/// Clears the entire registry.
		/// </summary>
		void clear();

	public:
		/// <summary>
		/// Registers the System, so the System can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The System to be registered.</typeparam>
		template <class T>
		static void register_system(String const& name);

		/// <summary>
		/// Returns a list of names of every System that have been registered.
		/// </summary>
		/// <returns></returns>
		static std::vector<String> get_registered_systems();

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	public:
		friend String to_string(SystemRegistry const& value);
	};

	template<class T>
	T* SystemRegistry::emplace(String const& name, int const priority)
	{
		return static_cast<T*>(this->emplace(name, new T(), priority));
	}

	template<class T>
	T* SystemRegistry::find(String const& name) const
	{
		auto found = _allSystems.find(name);

		if (found != _allSystems.end())
		{
			return static_cast<T*>(found->second);
		}

		return nullptr;
	}

	template<class T>
	T* SystemRegistry::find() const
	{
		auto found = _typeLookup.find(typeid(T));

		if (found != _typeLookup.end())
		{
			return static_cast<T*>(found->second);
		}

		return nullptr;
	}

	template<class T>
	void SystemRegistry::register_system(String const& name)
	{
		if (_systemTypes.contains(name))
		{
			MINTY_INFO(std::format("System {} already registered.", name));
			return;
		}

		_systemTypes.emplace(name, [](Scene& scene) { return new T(scene); });

		MINTY_INFO(std::format("Registered system {}.", name));
	}
}