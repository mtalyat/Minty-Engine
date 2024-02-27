#pragma once
#include "M_SceneObject.h"
#include "M_Entity.h"

#include "M_UUID.h"
#include "M_Object.h"
#include "M_Console.h"
#include <map>
#include <unordered_map>

namespace minty
{
	struct Component;
	class ScriptClass;

	class EntityRegistry
		: public SceneObject, public entt::registry
	{
	public:
		typedef std::function<Component* (EntityRegistry&, Entity const, String const&)> ComponentEmplaceFunc;
		typedef std::function<Component const* (EntityRegistry const&, Entity const, String const&)> ComponentGetFunc;
		typedef std::function<void(EntityRegistry&, Entity const, String const&)> ComponentEraseFunc;

	private:
		struct ComponentFuncs
		{
			ComponentEmplaceFunc emplace;
			ComponentGetFunc get;
			ComponentEraseFunc erase;
		};

		static std::map<String const, ComponentFuncs const> _components; // name -> creation/get funcs
		static std::map<uint32_t const, String const> _componentTypes; // type id index -> name

		std::unordered_map<UUID, Entity> _idToEntity;
		std::unordered_map<Entity, UUID> _entityToId;

		std::unordered_map<String, std::unordered_set<Entity>> _tags;
	public:
		EntityRegistry(Runtime& engine, ID const sceneId);

		~EntityRegistry();

		// move constructor
		EntityRegistry(EntityRegistry&& other) noexcept;

		EntityRegistry& operator=(EntityRegistry&& other) noexcept;

		/// <summary>
		/// Creates a new Entity with a new UUID.
		/// </summary>
		/// <returns>The new Entity created with an IDComponent.</returns>
		Entity create();

		/// <summary>
		/// Creates a new Entity with the given UUID.
		/// </summary>
		/// <param name="uuid">The ID of this Entity.</param>
		/// <returns>The new Entity with an IDComponent attached, with the given UUID.</returns>
		Entity create(UUID const uuid);

		/// <summary>
		/// Creates a new Entity with a new UUID and with the given name.
		/// </summary>
		/// <param name="name">The name of the Entity.</param>
		/// <returns>The new Entity with a NameComponent attached, with the given name.</returns>
		Entity create(String const& name);

		/// <summary>
		/// Creates a new Entity with the given name and UUID.
		/// </summary>
		/// <param name="name">The name of the Entity.</param>
		/// <param name="uuid">The ID of this Entity.</param>
		/// <returns>The new Entity with a NameComponent and IDComponent.</returns>
		Entity create(String const& name, UUID const uuid);

		void enable(Entity const entity);

		void disable(Entity const entity);

		void dirty(Entity const entity);

		void set_parent(Entity const entity, Entity const parentEntity);

		Entity get_parent(Entity const entity) const;

		size_t get_child_count(Entity const entity) const;

		Entity get_child(Entity const entity, size_t const index) const;

		void detach_children(Entity const entity);

		/// <summary>
		/// Marks the given Entity for destruction.
		/// </summary>
		/// <param name="entity"></param>
		void destroy(Entity const entity, bool const includeChildren = true);

		/// <summary>
		/// Marks the Component with the given name on the given Entity for destruction.
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="componentName"></param>
		void destroy(Entity const entity, String const& componentName);

		/// <summary>
		/// Destroys the given Entity right now.
		/// </summary>
		/// <param name="entity"></param>
		void destroy_immediate(Entity const entity, bool const includeChildren = true);

		/// <summary>
		/// Destroys all Entities marked with the Destroy component.
		/// </summary>
		void destroy_queued();

	private:
		void destroy_trigger_events(Entity const entity, bool const sceneLoaded);

	public:

		/// <summary>
		/// Destroys the given component from any Entity that has it.
		/// </summary>
		/// <typeparam name="Type"></typeparam>
		template<typename Type>
		void clear()
		{
			entt::registry::clear<Type>();
		}

		/// <summary>
		/// Destroys everything in this EntityRegistry.
		/// </summary>
		void clear();

		//~EntityRegistry();

		/// <summary>
		/// Finds the first Entity with the given name.
		/// </summary>
		/// <param name="string"></param>
		/// <returns>The Entity, or NULL_ENTITY if not found.</returns>
		Entity find_by_name(String const& string) const;

		/// <summary>
		/// Finds the first Entity with the given tag.
		/// </summary>
		/// <param name="tag"></param>
		/// <returns>The Entity, or NULL_ENTITY if not found.</returns>
		Entity find_by_tag(String const& tag) const;

		/// <summary>
		/// Finds all of the Entities with the given tag.
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		std::unordered_set<Entity> find_all_by_tag(String const& tag) const;

		/// <summary>
		/// Finds the Entity with the given UUID.
		/// </summary>
		/// <param name="uuid"></param>
		/// <returns>The Entity, or NULL_ENTITY if not found.</returns>
		Entity find_by_id(UUID const uuid) const;

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
		String get_name(Entity const entity) const;

		/// <summary>
		/// Sets the NameComponent name of the Entity. Emplaces a NameComponent if needed. If no name is given, then the NameComponent is removed.
		/// </summary>
		/// <param name="entity">The Entity to name.</param>
		/// <param name="name">The new name of the Entity.</param>
		void set_name(Entity const entity, String const& name);

		String get_tag(Entity const entity) const;

		/// <summary>
		/// Sets the TagComponent tag of the Entity. Emplaces a TagComponent if needed. If no name is given, then the TagComponent is removed.
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="tag"></param>
		void set_tag(Entity const entity, String const& tag);

		/// <summary>
		/// Gets the ID of the given Entity.
		/// </summary>
		/// <param name="entity">The Entity to get the ID from.</param>
		/// <returns>The ID of the Entity, or 0 if no ID was found.</returns>
		UUID get_id(Entity const entity) const;

		/// <summary>
		/// Emplaces the Component onto the entity, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity to set the Component onto.</param>
		/// <returns>A pointer to the newly emplaced Component, or null if an error occured.</returns>
		Component* emplace_by_name(String const& name, Entity const entity);

		/// <summary>
		/// Gets the Component, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity that has the Component.</param>
		/// <returns>The Component, or null if it does not exist.</returns>
		Component* get_by_name(String const& name, Entity const entity);

		/// <summary>
		/// Gets the Component, by name.
		/// </summary>
		/// <param name="name">The name of the Component.</param>
		/// <param name="entity">The Entity that has the Component.</param>
		/// <returns>The Component, or null if it does not exist.</returns>
		Component const* get_by_name(String const& name, Entity const entity) const;

		/// <summary>
		/// Gets all Components that are attached to the given Entity.
		/// </summary>
		/// <param name="entity">The Entity with the components.</param>
		/// <returns>A vector of components.</returns>
		std::vector<Component const*> get_all(Entity const entity) const;

		/// <summary>
		/// Erases the Component, by name.
		/// </summary>
		/// <param name="name">The name of the component.</param>
		/// <param name="entity">The Entity that has the Component.</param>
		void erase_by_name(String const& name, Entity const entity);

		/// <summary>
		/// Clones the given Entity and its components, and returns the new Entity.
		/// </summary>
		/// <param name="entity">The Entity to clone.</param>
		/// <returns>The cloned Entity.</returns>
		Entity clone(Entity const entity);

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

		friend String to_string(EntityRegistry const& value);

	public:
		/// <summary>
		/// Registers the Component, so the Component can be dynamically created by name.
		/// </summary>
		/// <typeparam name="T">The Component to register.</typeparam>
		/// <param name="name">The name of the Component to register.</param>
		template <class T>
		static void register_component(String const& name);

		/// <summary>
		/// Registers the Script, so the Script can be dynamically created by name.
		/// </summary>
		/// <param name="name">The name of the Script to register.</param>
		static void register_script(String const& name);

		/// <summary>
		/// Returns a list of names of every Component/Script that have been registered.
		/// </summary>
		/// <returns></returns>
		static std::vector<String> get_registered_components();

	public:
		void serialize(Writer& writer) const override;
		void serialize_entity(Writer& writer, Entity const entity) const;
		Node serialize_entity(Entity const entity) const;
		void deserialize(Reader const& reader) override;

		static bool is_name_empty(String const& name);

	private:
		void add_to_lookup(Entity const entity, UUID const id);
		void remove_from_lookup(Entity const entity);
	};

	template<class T>
	Entity EntityRegistry::find_by_type() const
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
	void EntityRegistry::register_component(String const& name)
	{
		if (_components.contains(name))
		{
			Console::info(std::format("Component {} already registered.", name));
			return;
		}

		// funcs
		ComponentFuncs funcs = {
			.emplace = [](EntityRegistry& registry, Entity const entity, String const& name) -> Component* { return &registry.emplace<T>(entity); },
			.get = [](EntityRegistry const& registry, Entity const entity, String const& name) -> Component const* { return registry.try_get<T>(entity); },
			.erase = [](EntityRegistry& registry, Entity const entity, String const& name) -> void { registry.erase<T>(entity); },
		};
		_components.emplace(name, funcs);

		entt::type_info info = entt::type_id<T>();

		// type names
		_componentTypes.emplace(info.index(), name);

		Console::info(std::format("Registered component {}.", name));
	}
}