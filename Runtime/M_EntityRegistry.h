#pragma once

#include "libraries/entt/entt.hpp"
#include "M_Component.h"
#include "M_Transform.h"
#include "M_Console.h"
#include "M_ISerializable.h"

namespace minty
{
	typedef entt::entity Entity;
	constexpr Entity NULL_ENTITY = entt::null;

	class EntityRegistry
		: public Object, public entt::registry, public ISerializable
	{
	public:
		typedef std::function<Component* (EntityRegistry* const, Entity const)> ComponentCreateFunc;
		typedef std::function<Component const* (EntityRegistry const* const, Entity const)> ComponentGetFunc;

	private:
		struct ComponentFuncs
		{
			ComponentCreateFunc create;
			ComponentGetFunc get;
		};

		static std::map<std::string const, ComponentFuncs const> _components; // name -> creation/get funcs
		static std::map<uint32_t const, std::string const> _componentTypes; // type id index -> name

	public:
		EntityRegistry();

		~EntityRegistry();

		// move constructor
		EntityRegistry(EntityRegistry&& other) noexcept;

		EntityRegistry& operator=(EntityRegistry&& other) noexcept;

		/// <summary>
		/// Creates a new Entity.
		/// </summary>
		/// <returns>The new Entity created.</returns>
		Entity create();

		/// <summary>
		/// Creates a new Entity with the given name on a NameComponent.
		/// </summary>
		/// <param name="name">The name of the Entity.</param>
		/// <returns>The new Entity with a NameComponent attached, with the given name.</returns>
		Entity create(std::string const& name);

		//~EntityRegistry();

		/// <summary>
		/// Finds the first Entity with the given name.
		/// </summary>
		/// <param name="string"></param>
		/// <returns>The Entity, if found, otherwise NULL_ENTITY.</returns>
		Entity find_by_name(std::string const& string) const;

		/// <summary>
		/// Finds the first Entity with the given Component type.
		/// </summary>
		/// <typeparam name="T">The type of component.</typeparam>
		/// <returns>The Entity, if found, otherwise NULL_ENTITY.</returns>
		template<class T>
		Entity find_by_type() const;

		/// <summary>
		/// Gets the name of the Entity, or an empty string if no name exists.
		/// The name of an Entity can be added by emplacing the NameComponent component.
		/// </summary>
		/// <param name="entity">The Entity to get the name from.</param>
		/// <returns>The name of the Entity, or "" if no name.</returns>
		std::string get_name(Entity const entity) const;

		/// <summary>
		/// Emplaces the Component onto the entity, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity to set the Component onto.</param>
		/// <returns>A pointer to the newly emplaced Component, or null if an error occured.</returns>
		Component* emplace_by_name(std::string const& name, Entity const entity);

		/// <summary>
		/// Gets the Component, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity that has the Component.</param>
		/// <returns>The Component, or null if it does not exist.</returns>
		Component const* get_by_name(std::string const& name, Entity const entity) const;

		/// <summary>
		/// Prints an Entity's values to the screen.
		/// </summary>
		/// <param name="entity"></param>
		void print(Entity const entity) const;

		/// <summary>
		/// Gets the total number of Entities within this registry.
		/// </summary>
		/// <returns>The total number of Entities.</returns>
		size_t size() const;

		friend std::string to_string(EntityRegistry const& value);

		/// <summary>
		/// Registers the Component, so the Component can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The Component to register.</typeparam>
		template <class T>
		static void register_component(std::string const& name);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	private:
		Node serialize_entity(Entity const entity) const;
	};

	template<class T>
	inline Entity EntityRegistry::find_by_type() const
	{
		// iterate through view
		for (auto [entity, t] : this->view<T const>().each())
		{
			// just return the first one
			return entity;
		}

		// no components on any entities
		return NULL_ENTITY;
	}

	template<class T>
	void EntityRegistry::register_component(std::string const& name)
	{
		// funcs
		ComponentFuncs funcs = {
			.create = [](EntityRegistry* const registry, Entity const entity) { return &registry->emplace<T>(entity); },
				.get = [](EntityRegistry const* const registry, Entity const entity) { return registry->try_get<T>(entity); }
		};
		_components.emplace(name, funcs);

		entt::type_info info = entt::type_id<T>();

		// type names
		_componentTypes.emplace(info.index(), name);

		console::info(std::format("Registered component {}", name));
	}
}