#pragma once

#include "libraries/entt/entt.hpp"
#include "M_Component.h"
#include "M_Console.h"

namespace minty
{
	typedef entt::entity Entity;
	constexpr Entity NULL_ENTITY = entt::null;

	class EntityRegistry
		: public Object, public entt::registry
	{
	public:
		typedef std::function<Component* (EntityRegistry* const, Entity const)> ComponentFunc;

	private:
		static std::map<std::string const, ComponentFunc const> _componentTypes;

	public:
		EntityRegistry();

		/// <summary>
		/// Gets the name of the Entity, or an empty string if no name exists.
		/// The name of an Entity can be added by emplacing the NameComponent component.
		/// </summary>
		/// <param name="entity">The Entity to get the name from.</param>
		/// <returns>The name of the Entity, or "" if no name.</returns>
		std::string get_name(Entity const entity) const;

		/// <summary>
		/// Emplaces the component onto the entity, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity to emplace the Component onto.</param>
		/// <returns>A pointer to the newly emplaced Component, or null if an error occured.</returns>
		Component* emplace_by_name(std::string const& name, Entity const entity);

		/// <summary>
		/// Gets the total number of Entities within this registry.
		/// </summary>
		/// <returns>The total number of Entities.</returns>
		size_t count() const;

		std::string const to_string() const override;

		/// <summary>
		/// Registers the Component, so the Component can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The Component to register.</typeparam>
		template <class T>
		static void register_component(std::string const& name);
	};

	template<class T>
	void EntityRegistry::register_component(std::string const& name)
	{
		_componentTypes.emplace(name, [](EntityRegistry* const registry, Entity const entity) { return &registry->emplace<T>(entity); });

		console::info(std::format("Registered component {}", name));
	}
}