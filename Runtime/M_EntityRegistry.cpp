#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_SerializationData.h"
#include "M_Console.h"
#include "M_Runtime.h"
#include "M_SceneManager.h"
#include "M_Scene.h"

#include "M_NameComponent.h"
#include "M_TagComponent.h"
#include "M_RelationshipComponent.h"
#include "M_TransformComponent.h"
#include "M_DirtyComponent.h"
#include "M_DestroyEntityComponent.h"
#include "M_DestroyComponentComponent.h"
#include "M_EnabledComponent.h"

#include "M_ScriptClass.h"
#include "M_ScriptObject.h"
#include "M_ScriptEngine.h"
#include "M_ScriptComponent.h"

#include "M_Reader.h"
#include "M_Writer.h"

#include <sstream>
#include <map>

using namespace minty;

std::map<String const, EntityRegistry::ComponentFuncs const> EntityRegistry::_components = std::map<String const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, String const> EntityRegistry::_componentTypes = std::map<uint32_t const, String const>();

minty::EntityRegistry::EntityRegistry(Runtime& engine, ID const sceneId)
	: SceneObject(engine, sceneId)
	, entt::registry()
	, _idToEntity()
	, _entityToId()
	, _tags()
{
	// TODO: this does not account for when somebody gets the component by reference and updates it that wey
	// make it so whenever a transform is editied, it is marked as dirty
	on_construct<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_update<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
}

minty::EntityRegistry::~EntityRegistry()
{
	// clear entities, if there are any
	// (making sure OnDestroy gets called)
	clear();
}

minty::EntityRegistry::EntityRegistry(EntityRegistry&& other) noexcept
	: entt::registry(std::move(other))
{}

EntityRegistry& minty::EntityRegistry::operator=(EntityRegistry&& other) noexcept
{
	entt::registry::operator=(std::move(other));

	return *this;
}

Entity minty::EntityRegistry::create()
{
	return create(UUID());
}

Entity minty::EntityRegistry::create(UUID const uuid)
{
	// create entity with the given UUID
	Entity entity = entt::registry::create();
	MINTY_ASSERT_FORMAT(!_idToEntity.contains(uuid), "There was a UUID collision with id: {}.", static_cast<uint64_t>(uuid));
	add_to_lookup(entity, uuid);
	return entity;
}

Entity minty::EntityRegistry::create(String const& name)
{
	return create(name, UUID());
}

Entity minty::EntityRegistry::create(String const& name, UUID const uuid)
{
	Entity entity = create(uuid);
	if (!name.empty()) emplace<NameComponent>(entity).name = name;
	return entity;
}

void minty::EntityRegistry::enable(Entity const entity)
{
	// add enabled
	if (!all_of<EnabledComponent>(entity))
	{
		emplace<EnabledComponent>(entity);

		// trigger event if loaded
		if (get_scene().is_loaded())
		{
			ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity);
			ScriptOnEnableComponent* eventComponent = try_get<ScriptOnEnableComponent>(entity);
			if (scriptComponent && eventComponent)
			{
				eventComponent->invoke(SCRIPT_METHOD_NAME_ONENABLE, *scriptComponent);
			}
		}
	}
}

void minty::EntityRegistry::disable(Entity const entity)
{
	// remove enabled
	if (all_of<EnabledComponent>(entity))
	{
		erase<EnabledComponent>(entity);

		// trigger event if loaded
		if (get_scene().is_loaded())
		{
			ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity);
			ScriptOnDisableComponent* eventComponent = try_get<ScriptOnDisableComponent>(entity);
			if (scriptComponent && eventComponent)
			{
				eventComponent->invoke(SCRIPT_METHOD_NAME_ONDISABLE, *scriptComponent);
			}
		}
	}
}

void minty::EntityRegistry::set_enabled(Entity const entity, bool const enabled)
{
	if (enabled)
	{
		enable(entity);
	}
	else
	{
		disable(entity);
	}
}

bool minty::EntityRegistry::get_enabled(Entity const entity) const
{
	return all_of<EnabledComponent>(entity);
}

void minty::EntityRegistry::dirty(Entity const entity)
{
	if (!all_of<DirtyComponent>(entity))
	{
		emplace<DirtyComponent>(entity);
	}
}

void minty::EntityRegistry::set_parent(Entity const entity, Entity const parentEntity)
{
	// get or emplace: this entity could not have a relationship
	RelationshipComponent& relationship = get_or_emplace<RelationshipComponent>(entity);

	// already the parent
	if (relationship.parent == parentEntity) return;

	// get old parent if any, remove from children
	if (relationship.parent != NULL_ENTITY)
	{
		RelationshipComponent& parentRelationship = get<RelationshipComponent>(relationship.parent);

		MINTY_ASSERT(parentRelationship.children >= 1);

		// if this is the last child, remove all references
		if (parentRelationship.children == 1)
		{
			parentRelationship.first = NULL_ENTITY;
			parentRelationship.last = NULL_ENTITY;
		}
		else
		{
			// if this is the first child, set the first child to the next sibling
			if (entity == parentRelationship.first)
			{
				RelationshipComponent& siblingRelationship = get<RelationshipComponent>(relationship.next);
				siblingRelationship.prev = NULL_ENTITY;

				parentRelationship.first = relationship.next;
			}

			// if this is the last child, set the last child to the prev sibling
			if (entity == parentRelationship.last)
			{
				RelationshipComponent& siblingRelationship = get<RelationshipComponent>(relationship.prev);
				siblingRelationship.prev = NULL_ENTITY;

				parentRelationship.last = relationship.prev;
			}
		}

		parentRelationship.children--;

		// if parent no longer has children, remove its component
		if (!parentRelationship.children)
		{
			erase<RelationshipComponent>(relationship.parent);
		}
	}

	// set new parent
	relationship.parent = parentEntity;
	relationship.prev = NULL_ENTITY;
	relationship.next = NULL_ENTITY;

	// because it is likely that the global position has changed, update that too
	dirty(entity);

	// update new parent's children
	if (relationship.parent != NULL_ENTITY)
	{
		// get or emplace here: the new parent could not have a relationship component
		RelationshipComponent& parentRelationship = get_or_emplace<RelationshipComponent>(relationship.parent);

		// add new child to end of list

		if (!parentRelationship.children)
		{
			// if children list is empty, add to front and back
			parentRelationship.first = entity;
			parentRelationship.last = entity;
		}
		else
		{
			// if children list is not empty, add to back
			RelationshipComponent& lastSiblingRelationship = get_or_emplace<RelationshipComponent>(parentRelationship.last);

			lastSiblingRelationship.next = entity;
			relationship.prev = parentRelationship.last;
			parentRelationship.last = entity;
		}

		parentRelationship.children++;
	}
}

Entity minty::EntityRegistry::get_parent(Entity const entity) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		return component->parent;
	}

	return NULL_ENTITY;
}

size_t minty::EntityRegistry::get_child_count(Entity const entity) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		return component->children;
	}

	return 0;
}

Entity minty::EntityRegistry::get_child(Entity const entity, size_t const index) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		Entity child = component->first;

		for (size_t i = 0; i < index && child != NULL_ENTITY; i++)
		{
			// get next entity
			RelationshipComponent const& childComponent = get<RelationshipComponent>(child);
			child = childComponent.next;
		}

		return child;
	}

	return NULL_ENTITY;
}

void minty::EntityRegistry::detach_children(Entity const entity)
{
	if (!all_of<RelationshipComponent>(entity))
	{
		// no relationship
		return;
	}

	RelationshipComponent& component = get<RelationshipComponent>(entity);

	// for each child, unset the parent
	while (component.first != NULL_ENTITY)
	{
		set_parent(component.first, NULL_ENTITY);
	}
}

void minty::EntityRegistry::destroy(Entity const entity, bool const includeChildren)
{
	if (!includeChildren)
	{
		// destroy the Entity only
		emplace_or_replace<DestroyEntityComponent>(entity);
		return;
	}

	// destroy self and all children
	std::vector<Entity> stack;
	stack.push_back(entity);

	while (!stack.empty())
	{
		// get the entity
		Entity e = stack.back();
		stack.pop_back();

		// add
		emplace_or_replace<DestroyEntityComponent>(e);

		// add children to stack
		if (RelationshipComponent* component = try_get<RelationshipComponent>(e))
		{
			e = component->first;

			while (e != NULL_ENTITY)
			{
				// add to stack
				stack.push_back(e);

				// move to next child
				e = component->next;
			}
		}
	}
}

void minty::EntityRegistry::destroy(Entity const entity, String const& componentName)
{
	DestroyComponentComponent& destroyComponent = get_or_emplace<DestroyComponentComponent>(entity);
	destroyComponent.components.emplace(componentName);
}

void minty::EntityRegistry::destroy_immediate(Entity const entity, bool const includeChildren)
{
	// remove from parent, if any
	set_parent(entity, NULL_ENTITY);

	if (!includeChildren)
	{
		// trigger events
		destroy_trigger_events(entity, get_scene().is_loaded());

		// remove from lookups
		remove_from_lookup(entity);

		// destroy entity
		entt::registry::destroy(entity);

		return;
	}

	std::vector<Entity> stack;
	stack.push_back(entity);

	bool loaded = get_scene().is_loaded();

	while (!stack.empty())
	{
		Entity e = stack.back();
		stack.pop_back();

		// trigger events
		destroy_trigger_events(e, loaded);

		// remove from lookups
		remove_from_lookup(entity);

		// destroy entity
		entt::registry::destroy(entity);

		// add children to stack
		if (RelationshipComponent* component = try_get<RelationshipComponent>(e))
		{
			e = component->first;

			while (e != NULL_ENTITY)
			{
				// add to stack
				stack.push_back(e);

				// move to next child
				e = component->next;
			}
		}
	}
}

void minty::EntityRegistry::destroy_queued()
{
	ScriptEngine& scriptEngine = get_runtime().get_script_engine();

	// call all events first

	bool loaded = get_scene().is_loaded();

	// destroy all components, as long as the entity itself is not being destroyed
	for (auto [entity, destroy, script] : view<DestroyComponentComponent const, ScriptComponent const>(entt::exclude<DestroyEntityComponent>).each())
	{
		// call events
		if (loaded)
		{
			if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity)) eventComp->invoke(SCRIPT_METHOD_NAME_ONDISABLE, script, destroy.components);
			if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity)) eventComp->invoke(SCRIPT_METHOD_NAME_ONUNLOAD, script, destroy.components);
		}
		if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity)) eventComp->invoke(SCRIPT_METHOD_NAME_ONDESTROY, script, destroy.components);

		// erase the script components
		for (auto const& name : destroy.components)
		{
			erase_by_name(name, entity);
		}
	}

	// erase all components w/o a script
	for (auto [entity, destroy] : view<DestroyComponentComponent const>(entt::exclude<DestroyEntityComponent>).each())
	{
		for (auto const& name : destroy.components)
		{
			// remove from entity
			erase_by_name(name, entity);
		}
	}

	// clear all tags, as the entity is still alive
	clear<DestroyComponentComponent>();

	// destroy entities with scripts
	for (auto [entity, destroy, script] : view<DestroyEntityComponent const, ScriptComponent>().each())
	{
		// call events
		destroy_trigger_events(entity, loaded);

		// remove from UUID lookup
		remove_from_lookup(entity);
	}

	// no scripts, so no events
	for (auto [entity, destroy] : view<DestroyEntityComponent const>(entt::exclude<ScriptComponent>).each())
	{
		// remove from UUID lookup
		remove_from_lookup(entity);
	}

	// get the entities that are ready for destruction, and destroy them all at once
	auto destroyView = view<DestroyEntityComponent const>();
	entt::registry::destroy(destroyView.begin(), destroyView.end());
}

void minty::EntityRegistry::destroy_trigger_events(Entity const entity, bool const sceneLoaded)
{
	if (ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity))
	{
		if (sceneLoaded)
		{
			if (all_of<EnabledComponent>(entity))
				if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity))
					eventComp->invoke(SCRIPT_METHOD_NAME_ONDISABLE, *scriptComponent);

			if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity))
				eventComp->invoke(SCRIPT_METHOD_NAME_ONUNLOAD, *scriptComponent);
		}
		if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity))
			eventComp->invoke(SCRIPT_METHOD_NAME_ONDESTROY, *scriptComponent);
	}
}

void minty::EntityRegistry::clear()
{
	// call OnDestroy on any scripts
	for (auto [entity, script, ondestroy] : view<ScriptComponent const, ScriptOnDestroyComponent const>().each())
	{
		ondestroy.invoke(SCRIPT_METHOD_NAME_ONDESTROY, script);
	}

	// destroy them all
	entt::registry::clear();

	// clear the lookup list
	_idToEntity.clear();
	_entityToId.clear();

	// clear tags
	_tags.clear();
}

Entity minty::EntityRegistry::find_by_name(String const& string) const
{
	if (is_name_empty(string))
	{
		return NULL_ENTITY;
	}

	for (auto [entity, name] : this->view<NameComponent const>().each())
	{
		if (name.name.compare(string) == 0)
		{
			// found entity with same name
			return entity;
		}
	}

	// did not find entity
	return NULL_ENTITY;
}

Entity minty::EntityRegistry::find_by_tag(String const& tag) const
{
	auto found = _tags.find(tag);

	if (found != _tags.end())
	{
		return *found->second.begin();
	}

	// no entities with the given tag
	return NULL_ENTITY;
}

std::unordered_set<Entity> minty::EntityRegistry::find_all_by_tag(String const& tag) const
{
	auto found = _tags.find(tag);

	if (found != _tags.end())
	{
		return found->second;
	}

	// no entities with the given tag
	return std::unordered_set<Entity>();
}

Entity minty::EntityRegistry::find_by_id(UUID const uuid) const
{
	auto found = _idToEntity.find(uuid);

	if (found != _idToEntity.end())
	{
		return found->second;
	}
	else
	{
		return NULL_ENTITY;
	}
}

String minty::EntityRegistry::get_name(Entity const entity) const
{
	// if entity is null, return NULL
	if (entity == NULL_ENTITY)
	{
		return "";
	}

	// if entity has a name component, get the name
	// otherwise return empty
	NameComponent const* name = try_get<NameComponent>(entity);

	if (name)
	{
		return name->name;
	}
	else
	{
		return "";
	}
}

String minty::EntityRegistry::get_name_safe(Entity const entity) const
{
	String name = get_name(entity);

	if (name.empty()) return "_";

	return name;
}

UUID minty::EntityRegistry::get_id(Entity const entity) const
{
	auto found = _entityToId.find(entity);

	if (found != _entityToId.end())
	{
		return found->second;
	}

	return UUID::create_empty();
}

void minty::EntityRegistry::set_name(Entity const entity, String const& name)
{
	// check if name is a real name, or empty
	if (name.empty())
	{
		// name is empty, remove the component if it exists
		if (any_of<NameComponent>(entity))
		{
			erase<NameComponent>(entity);
		}
	}
	else
	{
		// name is occupied
		NameComponent& nameComponent = get_or_emplace<NameComponent>(entity);
		nameComponent.name = name;
	}
}

String minty::EntityRegistry::get_tag(Entity const entity) const
{
	if (TagComponent const* tag = try_get<TagComponent const>(entity))
	{
		return tag->tag;
	}

	return Text::EMPTY;
}

void minty::EntityRegistry::set_tag(Entity const entity, String const& tag)
{
	// check if name is a real name, or empty
	if (tag.empty())
	{
		// name is empty, remove the component if it exists
		if (TagComponent* tagComponent = try_get<TagComponent>(entity))
		{
			// this should be in the tags lookup: if not, somebody meddled with the tag outside of this method
			MINTY_ASSERT(_tags.contains(tagComponent->tag));

			auto& set = _tags.at(tagComponent->tag);
			set.erase(entity);

			// if the set is now empty, remove tag from lookup
			if (set.empty())
			{
				_tags.erase(tagComponent->tag);
			}

			// erase component
			erase<TagComponent>(entity);
		}
	}
	else
	{
		// add component
		TagComponent& tagComponent = get_or_emplace<TagComponent>(entity);
		tagComponent.tag = tag;

		// add to tags lookup
		auto found = _tags.find(tag);

		if (found != _tags.end())
		{
			// existing
			found->second.emplace(entity);
		}
		else
		{
			// new
			_tags.emplace(tag, std::unordered_set<Entity> { entity });
		}
	}
}

Component* minty::EntityRegistry::emplace_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.emplace(*this, entity, name);
	}
}

Component* minty::EntityRegistry::get_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return const_cast<Component*>(static_cast<Component const*>(found->second.get(*this, entity, name)));
	}
}

Component const* minty::EntityRegistry::get_by_name(String const& name, Entity const entity) const
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name));
		return nullptr;
	}
	else
	{
		// name found
		return found->second.get(*this, entity, name);
	}
}

std::vector<Component const*> minty::EntityRegistry::get_all(Entity const entity) const
{
	std::vector<Component const*> components;

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
			{
				Console::error(std::format("Cannot find_animation component type with id: {}, name: {}", ctype.index(), ctype.name().data()));
				continue;
			}

			String name = found->second;

			components.push_back(this->get_by_name(found->second, entity));
		}
	}

	return components;
}

void minty::EntityRegistry::erase_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		Console::error(std::format("Cannot erase Component \"{}\". It has not been registered with the EntityRegistry.", name));
	}
	else
	{
		// name found
		found->second.erase(*this, entity, name);
	}
}

Entity minty::EntityRegistry::clone(Entity const entity)
{
	Entity newEntity = create();

	for (auto [id, storage] : this->storage()) {
		if (storage.contains(entity) && !storage.contains(newEntity)) {
			if (void* value = storage.value(entity))
			{
				storage.push(newEntity, value);
			}
		}
	}

	return newEntity;
}

void minty::EntityRegistry::print(Entity const entity) const
{
	// serialize entity, add it to parent, print that
	Console::print(serialize_entity(entity));
}

size_t minty::EntityRegistry::size() const
{
	return this->storage<Entity>()->size();
}

void minty::EntityRegistry::register_script(String const& name)
{
	if (_components.contains(name))
	{
		Console::info(std::format("Script {} already registered.", name));
		return;
	}

	// TODO: this can be generic for each type of script, so this does not need to be copied per script. 
	// Make this so it just uses the same ComponentFuncs

	// scripts work by adding/getting/removing from the ScriptComponent
	// funcs
	ComponentFuncs funcs = {
		.emplace = [](EntityRegistry& registry, Entity const entity, String const& name) -> Component*
		{
			// get the script component
			ScriptComponent* component = registry.try_get<ScriptComponent>(entity);

			// if component dne, add it quick
			if (!component)
			{
				component = &registry.emplace<ScriptComponent>(entity);
			}
			else if (component->scripts.contains(name))
			{
				// if the script component exists, do nothing
				return component;
			}

			// get the script engine
			ScriptEngine& engine = registry.get_runtime().get_script_engine();

			// get the script based on the name
			ScriptClass const* script = engine.search_for_class(name);

			MINTY_ASSERT_FORMAT(script != nullptr, "No script with name {} found.", name);

			// add a script object to it
			ID id = component->scripts.emplace(name, engine.create_object_component(UUID(), registry.get_id(entity), *script));
			ScriptObject& scriptObject = component->scripts.at(id);

			if (registry.get_runtime().get_mode() == Runtime::Mode::Normal)
			{
				// call OnCreate
				scriptObject.try_invoke(SCRIPT_METHOD_NAME_ONCREATE);

				// now add the helper components, if they are needed
				if (script->has_method(SCRIPT_METHOD_NAME_ONLOAD))
				{
					ScriptOnLoadComponent& eventComp = registry.get_or_emplace<ScriptOnLoadComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
				if (script->has_method(SCRIPT_METHOD_NAME_ONENABLE))
				{
					ScriptOnEnableComponent& eventComp = registry.get_or_emplace<ScriptOnEnableComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
				if (script->has_method(SCRIPT_METHOD_NAME_ONUPDATE))
				{
					ScriptOnUpdateComponent& eventComp = registry.get_or_emplace<ScriptOnUpdateComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
				if (script->has_method(SCRIPT_METHOD_NAME_ONDISABLE))
				{
					ScriptOnDisableComponent& eventComp = registry.get_or_emplace<ScriptOnDisableComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
				if (script->has_method(SCRIPT_METHOD_NAME_ONUNLOAD))
				{
					ScriptOnUnloadComponent& eventComp = registry.get_or_emplace<ScriptOnUnloadComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
				if (script->has_method(SCRIPT_METHOD_NAME_ONDESTROY))
				{
					ScriptOnDestroyComponent& eventComp = registry.get_or_emplace<ScriptOnDestroyComponent>(entity);
					eventComp.scriptIds.emplace(id);
				}
			}

			// return the script object
			return &component->scripts.at(id);
		},
		.get = [](EntityRegistry const& registry, Entity const entity, String const& name) -> Component const*
		{
			// get the component
			ScriptComponent const* component = registry.try_get<ScriptComponent>(entity);

			// if no component, no script
			if (!component) return nullptr;

			// check if script exists
			auto found = component->scripts.find(name);

			// if no script
			if (found == ERROR_ID)
			{
				return nullptr;
			}

			// found it, return the script object
			return &component->scripts.at(found);
		},
		.erase = [](EntityRegistry& registry, Entity const entity, String const& name) -> void
		{
			// get the script component
			ScriptComponent* component = registry.try_get<ScriptComponent>(entity);

			// hmm, no script to destroy
			if (!component) return;

			// get the ID of the script with the name
			ID id = component->scripts.get_id(name);

			// if not there, skip
			if (id == ERROR_ID) return;

			// get the event components and erase if needed
			if (ScriptOnLoadComponent* eventComp = registry.try_get<ScriptOnLoadComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnEnableComponent* eventComp = registry.try_get<ScriptOnEnableComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnUpdateComponent* eventComp = registry.try_get<ScriptOnUpdateComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnDisableComponent* eventComp = registry.try_get<ScriptOnDisableComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnUnloadComponent* eventComp = registry.try_get<ScriptOnUnloadComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnDestroyComponent* eventComp = registry.try_get<ScriptOnDestroyComponent>(entity)) eventComp->scriptIds.erase(id);

			// remove from scripts
			component->scripts.erase(id);

			// TODO: erase the scripts component if needed (including each on script event component)

			// get the script engine
			ScriptEngine& engine = registry.get_runtime().get_script_engine();

			// destroy the script object
			engine.destroy_object(component->id);
		},
	};
	_components.emplace(name, funcs);

	Console::info(std::format("Registered script {}.", name));
}

std::vector<String> minty::EntityRegistry::get_registered_components()
{
	std::vector<String> result;
	result.reserve(_components.size());

	for (auto const& pair : _components)
	{
		result.push_back(pair.first);
	}

	return result;
}

void minty::EntityRegistry::serialize(Writer& writer) const
{
	// write each entity, and each component under it
	String entityName;
	UUID entityId;

	String nodeName;
	String nodeValue;

	for (auto [entity] : this->storage<Entity>()->each())
	{
		// get name
		entityName = get_name(entity);

		// get ID
		entityId = get_id(entity);

		// if id is empty, generate one
		if (entityId.empty()) entityId = UUID();

		// if no name, just print ID
		if (entityName.empty())
		{
			// ID
			nodeName = to_string(entityId);
			nodeValue = Text::EMPTY;
		}
		else
		{
			// name: ID
			nodeName = entityName;
			nodeValue = to_string(entityId);
		}

		// serialize entity
		Node entityNode(nodeName, nodeValue);
		Writer entityWriter(entityNode, writer.get_data());

		serialize_entity(entityWriter, entity);

		// write entity node
		writer.write(entityNode);
	}
}

void minty::EntityRegistry::deserialize(Reader const& reader)
{
	// read each entity, add name if it has one

	Node const& node = reader.get_node();
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	ScriptEngine* scriptEngine = data.scene->get_runtime().get_engine<ScriptEngine>();

	MINTY_ASSERT(scriptEngine != nullptr);

	String name;
	String value;
	UUID id;

	Entity entity;

	// for each entity name given
	for (auto const& entityNode : node.get_children())
	{
		// get the name and ID from the node
		if (entityNode.has_data())
		{
			name = entityNode.get_name();
			value = entityNode.get_data();
		}
		else
		{
			// no data, so the "name" must be the ID, and there is no name
			name = Text::EMPTY;
			value = entityNode.get_name();
		}

		// convert value to ID, or generate a new one if needed
		if (!Parse::try_uuid(value, id))
		{
			// the value must be the name instead
			name = value;
			value = Text::EMPTY;

			// generate a new ID
			id = UUID();
		}

		// create entity in registry
		entity = create(name, id);

		// create it again for scripting
		scriptEngine->create_object_entity(id);

		// set entity in data for deserialization
		data.entity = entity;

		// cycle through each component on entity
		for (auto const& compNode : entityNode.get_children())
		{
			// cannot have multiple of same component
			if (get_by_name(compNode.get_name(), entity))
			{
				Console::error(std::format("Attempting to deserialize entity \"{}\" with multiple instances of the \"{}\" component.", entityNode.get_name(), compNode.get_name()));
				return;
			}

			// add component
			Component* comp = emplace_by_name(compNode.get_name(), entity);
			Reader compReader(compNode, &data);
			comp->deserialize(compReader);
		}
	}
}

bool minty::EntityRegistry::is_name_empty(String const& name)
{
	return name.size() == 0 || (name.size() == 1 && name.at(0) == '_');
}

void minty::EntityRegistry::add_to_lookup(Entity const entity, UUID const id)
{
	_idToEntity.emplace(id, entity);
	_entityToId.emplace(entity, id);
}

void minty::EntityRegistry::remove_from_lookup(Entity const entity)
{
	_idToEntity.erase(_entityToId.at(entity));
	_entityToId.erase(entity);
}

void minty::EntityRegistry::serialize_entity(Writer& writer, Entity const entity) const
{
	SerializationData data = *static_cast<SerializationData const*>(writer.get_data());
	data.entity = entity;
	Writer tempWriter(writer.get_node(), &data);

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
			{
				Console::error(std::format("Cannot find_animation component type with id: {}, name: {}", ctype.index(), ctype.name().data()));
				continue;
			}

			String name = found->second;

			// ignore NameComponent and IDComponent, those are used when writing the entity node
			if (name.compare("Name") == 0 || name.compare("ID") == 0)
			{
				continue;
			}

			// write component with its name and serialized values
			tempWriter.write(name, this->get_by_name(name, entity));
		}
	}
}

Node minty::EntityRegistry::serialize_entity(Entity const entity) const
{
	Node node(get_name(entity), to_string(get_id(entity)));
	SerializationData data
	{
		.scene = &get_scene(),
		.entity = entity,
	};
	Writer writer(node, &data);
	serialize_entity(writer, entity);
	return node;
}

String minty::to_string(Entity const value)
{
	return std::to_string(static_cast<unsigned int>(value));
}

String minty::to_string(EntityRegistry const& value)
{
	// total entities
	size_t entityCount = value.size();

	// named entities count
	size_t namedCount = value.view<NameComponent const>().size();

	// get a count of similarly named entities, so there isn't 100 of the same named entity
	std::map<String, size_t> counts;

	if (entityCount != namedCount)
	{
		// add unnamed
		counts.emplace("_", entityCount - namedCount);
	}

	if (namedCount > 0)
	{
		// add named
		for (auto&& [entity, name] : value.view<NameComponent const>().each())
		{
			if (counts.find(name.name) != counts.end())
			{
				// name exists
				counts[name.name] += 1;
			}
			else
			{
				// name does not exist yet
				counts[name.name] = 1;
			}
		}
	}

	// build string list

	std::stringstream stream;

	stream << "EntityRegistry(";

	size_t i = 0;
	for (auto const& pair : counts)
	{
		if (i > 0)
		{
			stream << ", ";
		}

		// add name
		stream << pair.first;

		// add number if more than one
		if (pair.second != 1)
		{
			stream << " (x" << pair.second << ")";
		}

		i++;
	}

	stream << ")";

	return stream.str();
}
