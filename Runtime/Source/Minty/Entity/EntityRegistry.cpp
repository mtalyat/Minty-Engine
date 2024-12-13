#include "pch.h"
#include "EntityRegistry.h"

#include "Minty/Component/AllComponents.h"
#include "Minty/Core/Application.h"
#include "Minty/Core/Console.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Entity/EntitySerializationData.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/Serialize/Reader.h"
#include "Minty/Serialize/Writer.h"
#include <sstream>
#include <map>

using namespace Minty;

std::map<String const, EntityRegistry::ComponentFuncs const> EntityRegistry::s_components = std::map<String const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, String const> EntityRegistry::s_componentTypes = std::map<uint32_t const, String const>();

Minty::EntityRegistry::EntityRegistry(Scene& scene)
	: SceneObject(scene)
	, entt::registry()
	, m_idToEntity()
	, m_entityToId()
	, m_tags()
{
	// TODO: this does not account for when somebody gets the component by reference and updates it that wey
	// make it so whenever a transform is editied, it is marked as dirty
	on_construct<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_update<TransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_construct<UITransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_update<UITransformComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_construct<CanvasComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
	on_update<CanvasComponent>().connect<&EntityRegistry::emplace_or_replace<DirtyComponent>>();
}

Minty::EntityRegistry::~EntityRegistry()
{}

Entity Minty::EntityRegistry::create()
{
	return create(UUID::create());
}

Entity Minty::EntityRegistry::create(UUID const uuid)
{
	// create entity with the given UUID
	Entity entity = entt::registry::create();
	MINTY_ASSERT_FORMAT(!m_idToEntity.contains(uuid), "There was a UUID collision with id: {}.", static_cast<uint64_t>(uuid));
	add_to_lookup(entity, uuid);
	return entity;
}

Entity Minty::EntityRegistry::create(String const& name)
{
	return create(name, UUID::create());
}

Entity Minty::EntityRegistry::create(String const& name, UUID const uuid)
{
	Entity entity = create(uuid);
	if (!name.empty()) emplace<NameComponent>(entity).name = name;
	return entity;
}

void Minty::EntityRegistry::enable(Entity const entity)
{
	// add enabled
	if (!all_of<EnabledComponent>(entity))
	{
		emplace<EnabledComponent>(entity);

		// trigger event if loaded
		if (get_scene().is_loaded())
		{
			ScriptOnEnableComponent* eventComponent = try_get<ScriptOnEnableComponent>(entity);
			if (eventComponent)
			{
				eventComponent->invoke();
			}
		}
	}
}

void Minty::EntityRegistry::disable(Entity const entity)
{
	// remove enabled
	if (all_of<EnabledComponent>(entity))
	{
		erase<EnabledComponent>(entity);

		// trigger event if loaded
		if (get_scene().is_loaded())
		{
			ScriptOnDisableComponent* eventComponent = try_get<ScriptOnDisableComponent>(entity);
			if (eventComponent)
			{
				eventComponent->invoke();
			}
		}
	}
}

void Minty::EntityRegistry::set_enabled(Entity const entity, Bool const enabled)
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

Bool Minty::EntityRegistry::get_enabled(Entity const entity) const
{
	return all_of<EnabledComponent>(entity);
}

void Minty::EntityRegistry::set_renderable(Entity const entity, Bool const renderable)
{
	if (renderable)
	{
		if (!all_of<RenderableComponent>(entity))
		{
			emplace<RenderableComponent>(entity);
		}
	}
	else
	{
		erase<RenderableComponent>(entity);
	}
}

Bool Minty::EntityRegistry::get_renderable(Entity const entity) const
{
	return all_of<RenderableComponent>(entity);
}

void Minty::EntityRegistry::dirty(Entity const entity)
{
	if (!all_of<DirtyComponent>(entity))
	{
		emplace<DirtyComponent>(entity);
	}

	if (!all_of<RelationshipComponent>(entity))
	{
		// no children
		return;
	}

	RelationshipComponent& relationship = get<RelationshipComponent>(entity);

	if (!relationship.children)
	{
		// no children
		return;
	}

	// dirty all children

	// TODO: make iterative
	Entity child = relationship.first;

	while (child != NULL_ENTITY)
	{
		dirty(child);

		RelationshipComponent const& childRelationship = get<RelationshipComponent const>(child);

		child = childRelationship.next;
	}
}

void Minty::EntityRegistry::fix_sibling_indices(Entity const startEntity, int const startIndex)
{
	Entity entity = startEntity;
	int i = startIndex;

	while (entity != NULL_ENTITY)
	{
		RelationshipComponent& relationship = get<RelationshipComponent>(entity);

		relationship.index = i;
		i++;

		entity = relationship.next;
	}
}

void Minty::EntityRegistry::set_parent(Entity const entity, Entity const parentEntity, uint32_t const insertionIndex)
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
			int index = relationship.index;
			Entity next = relationship.next;

			if (entity != parentRelationship.first && entity != parentRelationship.last)
			{
				// middle child: mend the gap
				RelationshipComponent& prevSiblingRelationship = get<RelationshipComponent>(relationship.prev);
				RelationshipComponent& nextSiblingRelationship = get<RelationshipComponent>(relationship.next);

				prevSiblingRelationship.next = relationship.next;
				nextSiblingRelationship.prev = relationship.prev;
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
					siblingRelationship.next = NULL_ENTITY;

					parentRelationship.last = relationship.prev;
				}
			}

			// fix following indices
			fix_sibling_indices(next, index + 1);
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
	relationship.index = 0;
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
		else if (insertionIndex == 0)
		{
			// insert at beginning
			RelationshipComponent& firstSiblingRelationship = get<RelationshipComponent>(parentRelationship.first);

			firstSiblingRelationship.prev = entity;
			relationship.next = parentRelationship.first;
			parentRelationship.first = entity;
			relationship.index = 0;

			fix_sibling_indices(relationship.next, 1);
		}
		else if (insertionIndex >= parentRelationship.children)
		{
			// add to end
			RelationshipComponent& lastSiblingRelationship = get<RelationshipComponent>(parentRelationship.last);

			lastSiblingRelationship.next = entity;
			relationship.prev = parentRelationship.last;
			parentRelationship.last = entity;
			relationship.index = lastSiblingRelationship.index + 1;
		}
		else
		{
			// insert into middle

			// get child in middle
			Entity middleChild = get_child(parentEntity, insertionIndex);

			// get surrounding
			RelationshipComponent& after = get<RelationshipComponent>(middleChild);
			RelationshipComponent& before = get<RelationshipComponent>(after.prev);

			// emplace into there
			relationship.prev = after.prev;
			relationship.next = middleChild;
			before.next = entity;
			after.prev = entity;
			relationship.index = after.index;

			fix_sibling_indices(relationship.next, relationship.index + 1);
		}

		parentRelationship.children++;
	}

	// if the entity has a UITransform, update its Canvas value
	if (UITransformComponent* uiTransform = try_get<UITransformComponent>(entity))
	{
		uiTransform->canvas = NULL_ENTITY;

		Entity parent = entity;

		while (parent != NULL_ENTITY)
		{
			// if parent has canvas, set value
			if (CanvasComponent* canvas = try_get<CanvasComponent>(parent))
			{
				uiTransform->canvas = parent;
				break;
			}

			// move to next parent
			RelationshipComponent const& parentRelationship = get<RelationshipComponent>(parent);
			parent = parentRelationship.parent;
		}
	}
}

Entity Minty::EntityRegistry::get_parent(Entity const entity) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		return component->parent;
	}

	return NULL_ENTITY;
}

void Minty::EntityRegistry::swap_siblings(Entity const left, Entity const right)
{
	if (left == NULL_ENTITY || right == NULL_ENTITY) return;

	MINTY_ASSERT(all_of<RelationshipComponent>(left));
	MINTY_ASSERT(all_of<RelationshipComponent>(right));

	// swapping self
	if (left == right) return;

	RelationshipComponent& leftR = get<RelationshipComponent>(left);
	RelationshipComponent& rightR = get<RelationshipComponent>(right);

	MINTY_ASSERT(leftR.parent != NULL_ENTITY);
	MINTY_ASSERT(leftR.parent == rightR.parent);

	// swap
	if (leftR.next == right)
	{
		Entity before = leftR.prev;
		Entity after = rightR.next;

		leftR.prev = right;
		leftR.next = rightR.next;

		rightR.prev = before;
		rightR.next = left;

		// fix surrounding
		if (RelationshipComponent* beforeR = try_get<RelationshipComponent>(before))
		{
			beforeR->next = right;
		}
		if (RelationshipComponent* afterR = try_get<RelationshipComponent>(after))
		{
			afterR->prev = left;
		}
	}
	else if (leftR.prev == right)
	{
		Entity before = rightR.prev;
		Entity after = leftR.next;

		leftR.prev = rightR.prev;
		leftR.next = right;

		rightR.prev = left;
		rightR.next = after;

		// fix surrounding
		if (RelationshipComponent* beforeR = try_get<RelationshipComponent>(before))
		{
			beforeR->next = left;
		}
		if (RelationshipComponent* afterR = try_get<RelationshipComponent>(after))
		{
			afterR->prev = right;
		}
	}
	else
	{
		// not connected at all
		Entity before1 = leftR.prev;
		Entity after1 = leftR.next;

		Entity before2 = rightR.prev;
		Entity after2 = rightR.next;

		leftR.prev = rightR.prev;
		leftR.next = rightR.next;

		rightR.prev = before1;
		rightR.next = after1;

		// fix surrounding
		if (RelationshipComponent* beforeR = try_get<RelationshipComponent>(before1))
		{
			beforeR->next = right;
		}
		if (RelationshipComponent* afterR = try_get<RelationshipComponent>(after1))
		{
			afterR->prev = right;
		}
		if (RelationshipComponent* beforeR = try_get<RelationshipComponent>(before2))
		{
			beforeR->next = left;
		}
		if (RelationshipComponent* afterR = try_get<RelationshipComponent>(after2))
		{
			afterR->prev = left;
		}
	}

	// swap indices
	int tempIndex = leftR.index;
	leftR.index = rightR.index;
	rightR.index = tempIndex;

	// update parent first and last if needed
	RelationshipComponent& parentR = get<RelationshipComponent>(leftR.parent);

	if (parentR.first == left) parentR.first = right;
	else if (parentR.first == right) parentR.first = left;

	if (parentR.last == left) parentR.last = right;
	else if (parentR.last == right) parentR.last = left;
}

void Minty::EntityRegistry::move_to_next(Entity const entity)
{
	if (!all_of<RelationshipComponent>(entity)) return;
	RelationshipComponent& relationship = get<RelationshipComponent>(entity);
	if (relationship.parent == NULL_ENTITY) return;

	swap_siblings(entity, relationship.next);
}

void Minty::EntityRegistry::move_to_previous(Entity const entity)
{
	if (!all_of<RelationshipComponent>(entity)) return;
	RelationshipComponent& relationship = get<RelationshipComponent>(entity);
	if (relationship.parent == NULL_ENTITY) return;

	swap_siblings(entity, relationship.prev);
}

void Minty::EntityRegistry::move_to_first(Entity const entity)
{
	if (!all_of<RelationshipComponent>(entity)) return;
	RelationshipComponent& relationship = get<RelationshipComponent>(entity);
	if (relationship.parent == NULL_ENTITY) return;

	// keep track of parent
	Entity parent = relationship.parent;

	RelationshipComponent& parentR = get<RelationshipComponent>(parent);

	// already the first child
	if (parentR.first == entity) return;

	// remove from children
	set_parent(entity, NULL_ENTITY);

	// add to beginning
	set_parent(entity, parent, 0);
}

void Minty::EntityRegistry::move_to_last(Entity const entity)
{
	if (!all_of<RelationshipComponent>(entity)) return;
	RelationshipComponent& relationship = get<RelationshipComponent>(entity);
	if (relationship.parent == NULL_ENTITY) return;

	// keep track of parent
	Entity parent = relationship.parent;

	RelationshipComponent& parentR = get<RelationshipComponent>(parent);

	// already the last child
	if (parentR.last == entity) return;

	// remove from children
	set_parent(entity, NULL_ENTITY);

	// add back at end
	set_parent(entity, parent);
}

std::vector<Entity> Minty::EntityRegistry::get_family_line(Entity const entity) const
{
	if (entity == NULL_ENTITY) return std::vector<Entity>();

	std::vector<Entity> result;
	result.push_back(entity);

	// if parent, get every parent
	if (RelationshipComponent const* relationship = try_get<RelationshipComponent>(entity))
	{
		Entity parent = relationship->parent;

		while (parent != NULL_ENTITY)
		{
			result.push_back(parent);

			relationship = try_get<RelationshipComponent>(parent);
			parent = relationship->parent;
		}
	}

	return result;
}

Size Minty::EntityRegistry::get_child_count(Entity const entity) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		return component->children;
	}

	return 0;
}

Entity Minty::EntityRegistry::get_child(Entity const entity, Size const index) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		Entity child = component->first;

		for (Size i = 0; i < index && child != NULL_ENTITY; i++)
		{
			// get next entity
			RelationshipComponent const& childComponent = get<RelationshipComponent>(child);
			child = childComponent.next;
		}

		return child;
	}

	return NULL_ENTITY;
}

void Minty::EntityRegistry::detach_children(Entity const entity)
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

void Minty::EntityRegistry::destroy(Entity const entity, Bool const includeChildren)
{
	if (!includeChildren)
	{
		// destroy the Entity only
		emplace_or_replace<DestroyComponent>(entity);
		return;
	}

	// destroy self and all children
	std::vector<Entity> stack;
	stack.push_back(entity);

	while (!stack.empty())
	{
		// get the entity
		Entity child = stack.back();
		stack.pop_back();

		// add
		emplace_or_replace<DestroyComponent>(child);

		// add children to stack
		if (RelationshipComponent* component = try_get<RelationshipComponent>(child))
		{
			child = component->first;

			while (child != NULL_ENTITY)
			{
				// add to stack
				stack.push_back(child);

				component = try_get<RelationshipComponent>(child);

				// move to next child
				child = component->next;
			}
		}
	}
}

void Minty::EntityRegistry::destroy(Entity const entity, String const& componentName)
{
	// if script, call events
	if (all_of<ScriptComponent>(entity))
	{
		Bool loaded = get_scene().is_loaded();

		ScriptComponent& script = get<ScriptComponent>(entity);

		// call events, if init events called
		if (!all_of<TriggerScriptEvents>(entity))
		{
			if (loaded)
			{
				if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity)) eventComp->invoke(componentName);
				if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity)) eventComp->invoke(componentName);
			}
			if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity)) eventComp->invoke(componentName);
		}
	}

	// erase the component
	erase_by_name(componentName, entity);
}

void Minty::EntityRegistry::destroy_immediate(Entity const entity, Bool const includeChildren)
{
	// remove from parent, if any
	set_parent(entity, NULL_ENTITY);

	Bool loaded = get_scene().is_loaded();

	if (!includeChildren)
	{
		// trigger events
		destroy_trigger_events(entity, loaded);

		// destroy script
		UUID id = get_id(entity);
		if (id.valid())
		{
			ScriptEngine::destroy_object(id);
		}

		// remove from lookups
		remove_from_lookup(entity);

		// destroy entity
		entt::registry::destroy(entity);

		return;
	}

	std::vector<Entity> stack;
	stack.push_back(entity);

	while (!stack.empty())
	{
		Entity e = stack.back();
		stack.pop_back();

		// add children to stack
		if (RelationshipComponent* component = try_get<RelationshipComponent>(e))
		{
			Entity child = component->first;

			while (child != NULL_ENTITY)
			{
				// add to stack
				stack.push_back(child);

				// update component
				component = try_get<RelationshipComponent>(child);

				// move to next child
				child = component->next;
			}
		}

		// trigger events
		destroy_trigger_events(e, loaded);

		// destroy script
		UUID id = get_id(entity);
		if (id.valid())
		{
			ScriptEngine::destroy_object(id);
		}

		// remove from lookups
		remove_from_lookup(e);

		// destroy entity
		entt::registry::destroy(e);
	}
}

void Minty::EntityRegistry::destroy_queued()
{
	// call all events first

	Bool loaded = get_scene().is_loaded();

	// destroy entities with scripts
	for (auto [entity, destroy, script] : view<DestroyComponent const, ScriptComponent>().each())
	{
		// call events
		destroy_trigger_events(entity, loaded);

		// destroy script
		UUID id = get_id(entity);
		if (id.valid())
		{
			ScriptEngine::destroy_object(id);
		}

		// remove from UUID lookup
		remove_from_lookup(entity);
	}

	// no scripts, so no events
	for (auto [entity, destroy] : view<DestroyComponent const>(entt::exclude<ScriptComponent>).each())
	{
		// remove from UUID lookup
		remove_from_lookup(entity);
	}

	// get the entities that are ready for destruction, and destroy them all at once
	auto destroyView = view<DestroyComponent const>();
	entt::registry::destroy(destroyView.begin(), destroyView.end());
}

void Minty::EntityRegistry::destroy_trigger_events(Entity const entity, Bool const sceneLoaded)
{
	if (try_get<ScriptComponent>(entity))
	{
		if (sceneLoaded)
		{
			if (all_of<EnabledComponent>(entity))
				if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity))
					eventComp->invoke();

			if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity))
				eventComp->invoke();
		}
		if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity))
			eventComp->invoke();
	}
}

void Minty::EntityRegistry::clear()
{
	// destroy the entities
	for (auto const& entity : storage<Entity>())
	{
		destroy_immediate(entity, false);
	}
	entt::registry::clear();

	// clear the lookup list and other data
	m_idToEntity.clear();
	m_entityToId.clear();
	m_tags.clear();
}

Entity Minty::EntityRegistry::find_by_name(String const& string) const
{
	if (string.empty())
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

Entity Minty::EntityRegistry::find_by_tag(String const& tag) const
{
	auto found = m_tags.find(tag);

	if (found != m_tags.end())
	{
		return *found->second.begin();
	}

	// no entities with the given tag
	return NULL_ENTITY;
}

std::unordered_set<Entity> Minty::EntityRegistry::find_all_by_tag(String const& tag) const
{
	auto found = m_tags.find(tag);

	if (found != m_tags.end())
	{
		return found->second;
	}

	// no entities with the given tag
	return std::unordered_set<Entity>();
}

Entity Minty::EntityRegistry::find_by_id(UUID const uuid) const
{
	auto found = m_idToEntity.find(uuid);

	if (found != m_idToEntity.end())
	{
		return found->second;
	}
	else
	{
		return NULL_ENTITY;
	}
}

String Minty::EntityRegistry::get_name(Entity const entity) const
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

String Minty::EntityRegistry::get_name_safe(Entity const entity) const
{
	String name = get_name(entity);

	if (name.empty()) return "_";

	return name;
}

UUID Minty::EntityRegistry::get_id(Entity const entity) const
{
	auto found = m_entityToId.find(entity);

	if (found != m_entityToId.end())
	{
		return found->second;
	}

	return INVALID_UUID;
}

void Minty::EntityRegistry::set_name(Entity const entity, String const& name)
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

String Minty::EntityRegistry::get_tag(Entity const entity) const
{
	if (TagComponent const* tag = try_get<TagComponent const>(entity))
	{
		return tag->tag;
	}

	return "";
}

void Minty::EntityRegistry::set_tag(Entity const entity, String const& tag)
{
	// check if name is a real name, or empty
	if (tag.empty())
	{
		// name is empty, remove the component if it exists
		if (TagComponent* tagComponent = try_get<TagComponent>(entity))
		{
			// this should be in the tags lookup: if not, somebody meddled with the tag outside of this method
			MINTY_ASSERT(m_tags.contains(tagComponent->tag));

			auto& set = m_tags.at(tagComponent->tag);
			set.erase(entity);

			// if the set is now empty, remove tag from lookup
			if (set.empty())
			{
				m_tags.erase(tagComponent->tag);
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
		auto found = m_tags.find(tag);

		if (found != m_tags.end())
		{
			// existing
			found->second.emplace(entity);
		}
		else
		{
			// new
			m_tags.emplace(tag, std::unordered_set<Entity> { entity });
		}
	}
}

Component* Minty::EntityRegistry::emplace_by_name(String const& name, Entity const entity)
{
	MINTY_ASSERT_FORMAT(s_components.contains(name), "Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name);

	Ref<ScriptClass> script = ScriptEngine::find_class(name);

	// add by Component name emplace func
	return s_components.at(name).emplace(*this, entity, script);
}

Component* Minty::EntityRegistry::get_by_name(String const& name, Entity const entity)
{
	auto found = s_components.find(name);
	if (found == s_components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name);
		return nullptr;
	}
	else
	{
		// name found
		Ref<ScriptClass> script = ScriptEngine::find_class(name);
		return const_cast<Component*>(static_cast<Component const*>(found->second.get(*this, entity, script)));
	}
}

Component const* Minty::EntityRegistry::get_by_name(String const& name, Entity const entity) const
{
	auto found = s_components.find(name);
	if (found == s_components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name);
		return nullptr;
	}
	else
	{
		// name found
		Ref<ScriptClass> script = ScriptEngine::find_class(name);
		return found->second.get(*this, entity, script);
	}
}

std::vector<Component const*> Minty::EntityRegistry::get_all(Entity const entity) const
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
			auto found = s_componentTypes.find(ctype.index());
			if (found == s_componentTypes.end())
			{
				MINTY_ERROR_FORMAT("Cannot find_animation component type with id: {}, name: {}", ctype.index(), ctype.name().data());
				continue;
			}

			String name = found->second;

			components.push_back(this->get_by_name(found->second, entity));
		}
	}

	return components;
}

void Minty::EntityRegistry::erase_by_name(String const& name, Entity const entity)
{
	auto found = s_components.find(name);
	if (found == s_components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot erase Component \"{}\". It has not been registered with the EntityRegistry.", name);
	}
	else
	{
		// name found
		Ref<ScriptClass> script = ScriptEngine::find_class(name);
		found->second.erase(*this, entity, script);
	}
}

Entity Minty::EntityRegistry::clone(Entity const entity)
{
	// serialize entity with new ID
	// TODO: switch to binary writer and binary reader
	DynamicContainer container{};
	TextMemoryWriter writer(&container);
	serialize_entity(writer, entity, UUID::create());

	// deserialize into a new Entity
	TextMemoryReader reader(&container);
	Entity clone = deserialize_entity(reader);

	return clone;
}

void Minty::EntityRegistry::print(Entity const entity) const
{
	// serialize entity
	DynamicContainer container{};
	TextMemoryWriter writer(&container);
	serialize_entity(writer, entity);

	// print the result from that
	Char eol = 0;
	container.append(&eol, sizeof(Char));
	String text(static_cast<Char const*>(container.data()));
	Console::print(text);
}

Size Minty::EntityRegistry::size() const
{
	return this->storage<Entity>()->size();
}

std::vector<Entity> Minty::EntityRegistry::get_dependents(Ref<Asset> const asset) const
{
	std::vector<Entity> result;

	// get type
	AssetType type = asset->get_type();

	// check based on the type
	// some types inherently have no dependents
	switch (type)
	{
	case AssetType::Sprite:
		// SpriteComponents use Sprites
		for (auto const& [entity, sprite] : view<SpriteComponent const>().each())
		{
			if (sprite.sprite == asset)
			{
				result.push_back(entity);
			}
		}
		break;
	case AssetType::Material:
		// MeshComponents use Materials
		for (auto const& [entity, mesh] : view<MeshComponent const>().each())
		{
			if (mesh.material == asset)
			{
				result.push_back(entity);
			}
		}
		break;
	case AssetType::Mesh:
		// MeshComponents use Meshes
		for (auto const& [entity, mesh] : view<MeshComponent const>().each())
		{
			if (mesh.mesh == asset)
			{
				result.push_back(entity);
			}
		}
		break;
	//case AssetType::Animator:
	//	// AnimatorComponent use Animators
	//	for (auto const& [entity, animator] : view<AnimatorComponent const>().each())
	//	{
	//		if (animator.animator.id() == asset->id())
	//		{
	//			result.push_back(entity);
	//		}
	//	}
	//	break;
	//case AssetType::AudioClip:
	//	// AudioSourceComponents use AudioClips
	//	for (auto const& [entity, source] : view<AudioSourceComponent>().each())
	//	{
	//		if (source.clip == asset)
	//		{
	//			result.push_back(entity);
	//		}
	//	}
	//	break;
	}

	return result;
}

void Minty::EntityRegistry::register_script(Ref<ScriptClass> const script)
{
	String name = script->get_full_name();

	if (s_components.contains(name))
	{
		MINTY_WARN_FORMAT("Script {} already registered.", name);
		return;
	}

	// TODO: this can be generic for each type of script, so this does not need to be copied per script. 
	// Make this so it just uses the same ComponentFuncs

	// scripts work by adding/getting/removing from the ScriptComponent
	// funcs
	ComponentFuncs funcs = {
		.emplace = [](EntityRegistry& registry, Entity const entity, Ref<ScriptClass> const script) -> Component*
		{
			MINTY_ASSERT_MESSAGE(script != nullptr, "Cannot emplace Component. Unknown Script.");

			// get the script component
			ScriptComponent* component = registry.try_get<ScriptComponent>(entity);

			UUID scriptId = script->id();

			// if component dne, add it quick
			if (!component)
			{
				component = &registry.emplace<ScriptComponent>(entity);
			}
			else if (component->scriptObjects.contains(scriptId))
			{
				// if the script component exists, do nothing
				return component;
			}

			// get entity object
			UUID entityId = registry.get_id(entity);
			Ref<ScriptObject> entityObject = ScriptEngine::get_or_create_object_entity(entityId);

			// add a script object to it
			Ref<ScriptObject> scriptObject = ScriptEngine::create_object_component(entityObject, script);
			component->scriptObjects.emplace(scriptId, script->get_full_name(), scriptObject);

			if (Application::instance().get_mode() == ApplicationMode::Normal)
			{
				// call OnCreate, if system is being used
				scriptObject->try_invoke(SCRIPT_METHOD_NAME_ONCREATE);

				// call load and enable right now if the scene is already loaded, otherwise do it later
				if (registry.get_scene().is_loaded())
				{
					registry.emplace_or_replace<TriggerScriptEvents>(entity);
				}

				// now add the helper components, if they are needed
				registry.connect_event<ScriptOnLoadComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONLOAD);
				registry.connect_event<ScriptOnEnableComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONENABLE);
				registry.connect_event<ScriptOnUpdateComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONUPDATE);
				registry.connect_event<ScriptOnFinalizeComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONFINALIZE);
				registry.connect_event<ScriptOnDisableComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONDISABLE);
				registry.connect_event<ScriptOnUnloadComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONUNLOAD);
				registry.connect_event<ScriptOnDestroyComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONDESTROY);
				registry.connect_event<ScriptOnPointerEnterComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERENTER);
				registry.connect_event<ScriptOnPointerHoverComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERHOVER);
				registry.connect_event<ScriptOnPointerExitComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTEREXIT);
				registry.connect_event<ScriptOnPointerMoveComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERMOVE);
				registry.connect_event<ScriptOnPointerDownComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERDOWN);
				registry.connect_event<ScriptOnPointerUpComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERUP);
				registry.connect_event<ScriptOnPointerClickComponent>(entity, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERCLICK);
			}

			// return the script object
			return scriptObject.get();
		},
		.get = [](EntityRegistry const& registry, Entity const entity, Ref<ScriptClass> const script) -> Component const*
		{
			MINTY_ASSERT_MESSAGE(script != nullptr, "Cannot get Component. Unknown Script.");

			// get the component
			ScriptComponent const* component = registry.try_get<ScriptComponent>(entity);

			// if no component, no script
			if (!component) return nullptr;

			// get id
			UUID scriptId = script->id();

			// check if script exists
			if (!component->scriptObjects.contains(scriptId))
			{
				return nullptr;
			}

			// found it, return the script object
			return component->scriptObjects.at(scriptId).get();
		},
		.erase = [](EntityRegistry& registry, Entity const entity, Ref<ScriptClass> const script) -> void
		{
			MINTY_ASSERT_MESSAGE(script != nullptr, "Cannot erase Component. Unknown Script.");

			// get the script component
			ScriptComponent* component = registry.try_get<ScriptComponent>(entity);

			UUID scriptId = script->id();

			// exit if no component or no corresponding ScriptObject for the given name
			if (!component || !component->scriptObjects.contains(scriptId)) return;

			// get the event components and erase if needed
			if (ScriptOnLoadComponent* eventComp = registry.try_get<ScriptOnLoadComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnEnableComponent* eventComp = registry.try_get<ScriptOnEnableComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnUpdateComponent* eventComp = registry.try_get<ScriptOnUpdateComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnDisableComponent* eventComp = registry.try_get<ScriptOnDisableComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnUnloadComponent* eventComp = registry.try_get<ScriptOnUnloadComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnDestroyComponent* eventComp = registry.try_get<ScriptOnDestroyComponent>(entity)) eventComp->scriptMethods.erase(scriptId);

			if (ScriptOnPointerEnterComponent* eventComp = registry.try_get<ScriptOnPointerEnterComponent>(entity)) eventComp->scriptMethods.erase(scriptId);
			if (ScriptOnPointerExitComponent* eventComp = registry.try_get<ScriptOnPointerExitComponent>(entity)) eventComp->scriptMethods.erase(scriptId);

			// remove from scripts
			component->scriptObjects.erase(scriptId);

			// TODO: erase the scripts component if needed (including each on script event component)

			// destroy the script object, if there is one
			if (ScriptEngine::contains_object(component->id))
			{
				ScriptEngine::destroy_object(component->id);
			}
		},
	};
	s_components.emplace(name, funcs);

	MINTY_INFO_FORMAT("Registered script {}.", name);
}

std::vector<String> Minty::EntityRegistry::get_registered_components()
{
	std::vector<String> result;
	result.reserve(s_components.size());

	for (auto const& pair : s_components)
	{
		result.push_back(pair.first);
	}

	return result;
}

void Minty::EntityRegistry::serialize(Writer& writer) const
{
	// serialize all with relationship, then others, since order matters with relationships
	for (auto [entity, relationship] : this->view<RelationshipComponent>().each())
	{
		// serialize entity and its data
		serialize_entity(writer, entity);
	}

	// serialize all without relationship, order does not matter
	for (auto [entity] : view<Entity>().each())
	{
		// ignore invalid or entities we have already written
		if (!valid(entity) || all_of<RelationshipComponent>(entity)) continue;

		serialize_entity(writer, entity);
	}
}

void Minty::EntityRegistry::deserialize(Reader& reader)
{
	// read each entity
	for (Size i = 0; i < reader.size(); i++)
	{
		deserialize_entity(reader, i);
	}
}

void Minty::EntityRegistry::serialize_entity(Writer& writer, Entity const entity, UUID const idOverride) const
{
	// get name
	String name = get_name(entity);

	// get ID, use override if it is valid, otherwise use existing
	UUID id = idOverride.valid() ? idOverride : get_id(entity);

	// if id is empty, generate new one
	if (!id.valid()) id = UUID::create();

	// name and ID
	writer.write(name, id);

	// set user data
	EntitySerializationData data{};
	data.entity = entity;
	data.scene = &get_scene();
	writer.push_data(&data);

	// write component data
	writer.indent();

	for (auto&& curr : this->storage())
	{
		//auto cid = curr.first;
		auto& storage = curr.second;
		auto const& ctype = storage.type();

		if (storage.contains(entity))
		{
			// this entity has this component type, so get the "pretty" name
			auto found = s_componentTypes.find(ctype.index());
			if (found == s_componentTypes.end())
			{
				continue;
			}

			if (ctype == entt::type_id<NameComponent>())
			{
				continue;
			}

			String name = found->second;

			// write component and its data
			writer.write(name);

			writer.indent();

			Component const* component = this->get_by_name(name, entity);
			component->serialize(writer);
			
			writer.outdent();
		}
	}

	writer.outdent();

	// unset data
	writer.pop_data();
}

Entity Minty::EntityRegistry::deserialize_entity(Reader& reader, Size const index)
{
	String name; 
	reader.read_name(index, name);
	String value;
	reader.read(index, value);

	// step into Entity
	reader.indent(index);

	UUID id;

	// convert value to ID, or generate a new one if needed
	if (!Parse::try_uuid(value, id))
	{
		value = "";

		// generate a new ID
		id = UUID::create();
	}

	// create entity in registry
	Entity entity = create(name, id);

	// create it again for scripting
	ScriptEngine::create_object_entity(id);

	// set user data
	EntitySerializationData data{};
	data.entity = entity;
	data.scene = &get_scene();
	reader.push_data(&data);

	// cycle through each component on entity
	String componentName;
	Component* component;
	for (Size i = 0; i < reader.size(); i++)
	{
		reader.read_name(i, componentName);

		// cannot parse multiple of same name
		MINTY_ASSERT_FORMAT(get_by_name(componentName, entity) == nullptr, "Attempting to deserialize entity \"{}\" with multiple instances of the \"{}\" component.", name, componentName);

		// add component with name and deserialize
		reader.indent(i);

		component = emplace_by_name(componentName, entity);
		component->deserialize(reader);

		reader.outdent();
	}

	// step out of entity
	reader.outdent();

	reader.pop_data();

	return entity;
}

void Minty::EntityRegistry::add_to_lookup(Entity const entity, UUID const id)
{
	m_idToEntity.emplace(id, entity);
	m_entityToId.emplace(entity, id);
}

void Minty::EntityRegistry::remove_from_lookup(Entity const entity)
{
	m_idToEntity.erase(m_entityToId.at(entity));
	m_entityToId.erase(entity);
}

String Minty::to_string(EntityRegistry const& value)
{
	// total entities
	Size entityCount = value.size();

	// named entities count
	Size namedCount = value.view<NameComponent const>().size();

	// get a count of similarly named entities, so there isn't 100 of the same named entity
	std::map<String, Size> counts;

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

	Size i = 0;
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
