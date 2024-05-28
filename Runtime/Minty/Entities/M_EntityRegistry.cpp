#include "pch.h"
#include "Minty/Entities/M_EntityRegistry.h"

#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Scenes/M_SceneManager.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Core/M_Application.h"

#include "Minty/Components/M_NameComponent.h"
#include "Minty/Components/M_TagComponent.h"
#include "Minty/Components/M_RelationshipComponent.h"
#include "Minty/Components/M_TransformComponent.h"
#include "Minty/UI/M_UITransformComponent.h"
#include "Minty/UI/M_CanvasComponent.h"
#include "Minty/Components/M_DirtyComponent.h"
#include "Minty/Components/M_DestroyEntityComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Rendering/M_RenderableComponent.h"
#include "Minty/Rendering/M_SpriteComponent.h"
#include "Minty/Rendering/M_Sprite.h"
#include "Minty/Rendering/M_MeshComponent.h"
#include "Minty/Rendering/M_Material.h"
#include "Minty/Animation/M_AnimatorComponent.h"
#include "Minty/Animation/M_Animator.h"
#include "Minty/Audio/M_AudioSourceComponent.h"
#include "Minty/Audio/M_AudioClip.h"

#include "Minty/Scripting/M_ScriptClass.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Scripting/M_ScriptComponent.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

#include <sstream>
#include <map>

using namespace Minty;

std::map<String const, EntityRegistry::ComponentFuncs const> EntityRegistry::_components = std::map<String const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, String const> EntityRegistry::_componentTypes = std::map<uint32_t const, String const>();

Minty::EntityRegistry::EntityRegistry(Scene& scene)
	: SceneObject(scene)
	, entt::registry()
	, _idToEntity()
	, _entityToId()
	, _tags()
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
{
	// clear entities, if there are any
	// (making sure OnDestroy gets called)
	clear();
}

Entity Minty::EntityRegistry::create()
{
	return create(UUID());
}

Entity Minty::EntityRegistry::create(UUID const uuid)
{
	// create entity with the given UUID
	Entity entity = entt::registry::create();
	MINTY_ASSERT_FORMAT(!_idToEntity.contains(uuid), "There was a UUID collision with id: {}.", static_cast<uint64_t>(uuid));
	add_to_lookup(entity, uuid);
	return entity;
}

Entity Minty::EntityRegistry::create(String const& name)
{
	return create(name, UUID());
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
			ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity);
			ScriptOnEnableComponent* eventComponent = try_get<ScriptOnEnableComponent>(entity);
			if (scriptComponent && eventComponent)
			{
				eventComponent->invoke(*scriptComponent);
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
			ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity);
			ScriptOnDisableComponent* eventComponent = try_get<ScriptOnDisableComponent>(entity);
			if (scriptComponent && eventComponent)
			{
				eventComponent->invoke(*scriptComponent);
			}
		}
	}
}

void Minty::EntityRegistry::set_enabled(Entity const entity, bool const enabled)
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

bool Minty::EntityRegistry::get_enabled(Entity const entity) const
{
	return all_of<EnabledComponent>(entity);
}

void Minty::EntityRegistry::set_renderable(Entity const entity, bool const renderable)
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

bool Minty::EntityRegistry::get_renderable(Entity const entity) const
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
		else if(insertionIndex == 0)
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

size_t Minty::EntityRegistry::get_child_count(Entity const entity) const
{
	if (RelationshipComponent const* component = try_get<RelationshipComponent>(entity))
	{
		return component->children;
	}

	return 0;
}

Entity Minty::EntityRegistry::get_child(Entity const entity, size_t const index) const
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

void Minty::EntityRegistry::destroy(Entity const entity, bool const includeChildren)
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
		Entity child = stack.back();
		stack.pop_back();

		// add
		emplace_or_replace<DestroyEntityComponent>(child);

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
		bool loaded = get_scene().is_loaded();

		ScriptComponent& script = get<ScriptComponent>(entity);

		// call events, if init events called
		if (!all_of<TriggerScriptEvents>(entity))
		{
			if (loaded)
			{
				if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity)) eventComp->invoke(script, componentName);
				if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity)) eventComp->invoke(script, componentName);
			}
			if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity)) eventComp->invoke(script, componentName);
		}

	}

	// erase the component
	erase_by_name(componentName, entity);
}

void Minty::EntityRegistry::destroy_immediate(Entity const entity, bool const includeChildren)
{
	// remove from parent, if any
	set_parent(entity, NULL_ENTITY);

	bool loaded = get_scene().is_loaded();

	if (!includeChildren)
	{
		// trigger events
		destroy_trigger_events(entity, loaded);

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

		// remove from lookups
		remove_from_lookup(e);

		// destroy entity
		entt::registry::destroy(e);
	}
}

void Minty::EntityRegistry::destroy_queued()
{
	ScriptEngine& scriptEngine = ScriptEngine::instance();

	// call all events first

	bool loaded = get_scene().is_loaded();

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

void Minty::EntityRegistry::destroy_trigger_events(Entity const entity, bool const sceneLoaded)
{
	if (ScriptComponent* scriptComponent = try_get<ScriptComponent>(entity))
	{
		if (sceneLoaded)
		{
			if (all_of<EnabledComponent>(entity))
				if (ScriptOnDisableComponent* eventComp = try_get<ScriptOnDisableComponent>(entity))
					eventComp->invoke(*scriptComponent);

			if (ScriptOnUnloadComponent* eventComp = try_get<ScriptOnUnloadComponent>(entity))
				eventComp->invoke(*scriptComponent);
		}
		if (ScriptOnDestroyComponent* eventComp = try_get<ScriptOnDestroyComponent>(entity))
			eventComp->invoke(*scriptComponent);
	}
}

void Minty::EntityRegistry::clear()
{
	// call OnDestroy on any scripts
	for (auto [entity, script, ondestroy] : view<ScriptComponent const, ScriptOnDestroyComponent const>().each())
	{
		ondestroy.invoke(script);
	}

	// destroy them all
	entt::registry::clear();

	// clear the lookup list
	_idToEntity.clear();
	_entityToId.clear();

	// clear tags
	_tags.clear();
}

Entity Minty::EntityRegistry::find_by_name(String const& string) const
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

Entity Minty::EntityRegistry::find_by_tag(String const& tag) const
{
	auto found = _tags.find(tag);

	if (found != _tags.end())
	{
		return *found->second.begin();
	}

	// no entities with the given tag
	return NULL_ENTITY;
}

std::unordered_set<Entity> Minty::EntityRegistry::find_all_by_tag(String const& tag) const
{
	auto found = _tags.find(tag);

	if (found != _tags.end())
	{
		return found->second;
	}

	// no entities with the given tag
	return std::unordered_set<Entity>();
}

Entity Minty::EntityRegistry::find_by_id(UUID const uuid) const
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
	auto found = _entityToId.find(entity);

	if (found != _entityToId.end())
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

Component* Minty::EntityRegistry::emplace_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot emplace Component \"{}\". It has not been registered with the EntityRegistry.", name);
		return nullptr;
	}
	else
	{
		// name found
		return found->second.emplace(*this, entity, name);
	}
}

Component* Minty::EntityRegistry::get_by_name(String const& name, Entity const entity)
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name);
		return nullptr;
	}
	else
	{
		// name found
		return const_cast<Component*>(static_cast<Component const*>(found->second.get(*this, entity, name)));
	}
}

Component const* Minty::EntityRegistry::get_by_name(String const& name, Entity const entity) const
{
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot get Component \"{}\". It has not been registered with the EntityRegistry.", name);
		return nullptr;
	}
	else
	{
		// name found
		return found->second.get(*this, entity, name);
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
			auto found = _componentTypes.find(ctype.index());
			if (found == _componentTypes.end())
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
	auto found = _components.find(name);
	if (found == _components.end())
	{
		// name not found
		MINTY_ERROR_FORMAT("Cannot erase Component \"{}\". It has not been registered with the EntityRegistry.", name);
	}
	else
	{
		// name found
		found->second.erase(*this, entity, name);
	}
}

Entity Minty::EntityRegistry::clone(Entity const entity)
{
	// serialize entity
	Node data = serialize_entity(entity);

	// change its id
	data.set_data(to_string(UUID()));

	// deserialize
	Entity clone = deserialize_entity(data);

	return clone;
}

void Minty::EntityRegistry::print(Entity const entity) const
{
	// serialize entity, add it to parent, print that
	Console::print(serialize_entity(entity));
}

size_t Minty::EntityRegistry::size() const
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
	case AssetType::Animator:
		// AnimatorComponent use Animators
		for (auto const& [entity, animator] : view<AnimatorComponent const>().each())
		{
			if (animator.animator.get_id() == asset->get_id())
			{
				result.push_back(entity);
			}
		}
		break;
	case AssetType::AudioClip:
		// AudioSourceComponents use AudioClips
		for (auto const& [entity, source] : view<AudioSourceComponent>().each())
		{
			if (source.clip == asset)
			{
				result.push_back(entity);
			}
		}
		break;
	}

	return result;
}

void Minty::EntityRegistry::register_script(String const& name)
{
	if (_components.contains(name))
	{
		MINTY_INFO_FORMAT("Script {} already registered.", name);
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
			ScriptEngine& engine = ScriptEngine::instance();

			// get the script based on the name
			ScriptClass const* script = engine.find_class(name);

			MINTY_ASSERT_FORMAT(script != nullptr, "No script with name {} found.", name);

			// add a script object to it
			ID id = component->scripts.emplace(name, engine.create_object_component(UUID(), registry.get_id(entity), *script));
			ScriptObject& scriptObject = component->scripts.at(id);

			if (Application::instance().get_mode() == ApplicationMode::Normal)
			{
				// call OnCreate
				scriptObject.try_invoke(SCRIPT_METHOD_NAME_ONCREATE);

				// call load and enable right now if the scene is already loaded, otherwise do it later
				if (registry.get_scene().is_loaded())
				{
					registry.emplace_or_replace<TriggerScriptEvents>(entity);
				}

				// now add the helper components, if they are needed
				registry.connect_event<ScriptOnLoadComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONLOAD);
				registry.connect_event<ScriptOnEnableComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONENABLE);
				registry.connect_event<ScriptOnUpdateComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONUPDATE);
				registry.connect_event<ScriptOnDisableComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONDISABLE);
				registry.connect_event<ScriptOnUnloadComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONUNLOAD);
				registry.connect_event<ScriptOnDestroyComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONDESTROY);
				registry.connect_event<ScriptOnPointerEnterComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERENTER);
				registry.connect_event<ScriptOnPointerHoverComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERHOVER);
				registry.connect_event<ScriptOnPointerExitComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTEREXIT);
				registry.connect_event<ScriptOnPointerMoveComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERMOVE);
				registry.connect_event<ScriptOnPointerDownComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERDOWN);
				registry.connect_event<ScriptOnPointerUpComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERUP);
				registry.connect_event<ScriptOnPointerClickComponent>(entity, id, scriptObject, SCRIPT_METHOD_NAME_ONPOINTERCLICK);
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

			if (ScriptOnPointerEnterComponent* eventComp = registry.try_get<ScriptOnPointerEnterComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnPointerExitComponent* eventComp = registry.try_get<ScriptOnPointerExitComponent>(entity)) eventComp->scriptIds.erase(id);

			// remove from scripts
			component->scripts.erase(id);

			// TODO: erase the scripts component if needed (including each on script event component)


			// destroy the script object, if there is one
			ScriptEngine& scripts = ScriptEngine::instance();
			scripts.destroy_object(component->id);
		},
	};
	_components.emplace(name, funcs);

	MINTY_INFO_FORMAT("Registered script {}.", name);
}

std::vector<String> Minty::EntityRegistry::get_registered_components()
{
	std::vector<String> result;
	result.reserve(_components.size());

	for (auto const& pair : _components)
	{
		result.push_back(pair.first);
	}

	return result;
}

void Minty::EntityRegistry::serialize(Writer& writer) const
{
	// write each entity, and each component under it
	String entityName;
	UUID entityId;

	String nodeName;
	String nodeValue;

	// serialize all with relationship, then others
	for (auto [entity, relationship] : this->view<RelationshipComponent>().each())
	{
		// get name
		entityName = get_name(entity);

		// get ID
		entityId = get_id(entity);

		// if id is empty, generate one
		if (!entityId) entityId = UUID();

		// if no name, just print ID
		if (entityName.empty())
		{
			// ID
			nodeName = to_string(entityId);
			nodeValue = "";
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

	// serialize all without relationship
	for (auto [entity] : view<Entity>().each())
	{
		// ignore invalid or entities we have already written
		if (!valid(entity) || all_of<RelationshipComponent>(entity)) continue;

		// get name
		entityName = get_name(entity);

		// get ID
		entityId = get_id(entity);

		// if id is empty, generate one
		if (!entityId) entityId = UUID();

		// if no name, just print ID
		if (entityName.empty())
		{
			// ID
			nodeName = to_string(entityId);
			nodeValue = "";
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

void Minty::EntityRegistry::deserialize(Reader const& reader)
{
	// read each entity, add name if it has one

	Node const& node = reader.get_node();

	ScriptEngine& scriptEngine = ScriptEngine::instance();

	// for each entity name given
	for (Node const& entityNode : node.get_children())
	{
		deserialize_entity(entityNode);
	}
}

Entity Minty::EntityRegistry::deserialize_entity(Node const& entityNode)
{
	String name;
	String value;
	UUID id;

	// get the name and ID from the node
	if (entityNode.has_data())
	{
		name = entityNode.get_name();
		value = entityNode.get_data();
	}
	else
	{
		// no data, so the "name" must be the ID, and there is no name
		name = "";
		value = entityNode.get_name();
	}

	// convert value to ID, or generate a new one if needed
	if (!Parse::try_uuid(value, id))
	{
		// the value must be the name instead
		name = value;
		value = "";

		// generate a new ID
		id = UUID();
	}

	// create entity in registry
	Entity entity = create(name, id);

	// create it again for scripting
	ScriptEngine& scriptEngine = ScriptEngine::instance();
	scriptEngine.create_object_entity(id);

	// set entity in data for deserialization
	SerializationData data
	{
		.scene = &get_scene(),
		.entity = entity
	};

	// cycle through each component on entity
	for (auto const& compNode : entityNode.get_children())
	{
		// cannot have multiple of same component
		if (get_by_name(compNode.get_name(), entity))
		{
			MINTY_ERROR_FORMAT("Attempting to deserialize entity \"{}\" with multiple instances of the \"{}\" component.", entityNode.get_name(), compNode.get_name());
			return entity;
		}

		// add component
		Component* comp = emplace_by_name(compNode.get_name(), entity);
		Reader compReader(compNode, &data);
		comp->deserialize(compReader);
	}

	return entity;
}

bool Minty::EntityRegistry::is_name_empty(String const& name)
{
	return name.size() == 0 || (name.size() == 1 && name.at(0) == '_');
}

void Minty::EntityRegistry::add_to_lookup(Entity const entity, UUID const id)
{
	_idToEntity.emplace(id, entity);
	_entityToId.emplace(entity, id);
}

void Minty::EntityRegistry::remove_from_lookup(Entity const entity)
{
	_idToEntity.erase(_entityToId.at(entity));
	_entityToId.erase(entity);
}

void Minty::EntityRegistry::serialize_entity(Writer& writer, Entity const entity) const
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
				continue;
			}

			if (ctype == entt::type_id<NameComponent>())
			{
				continue;
			}

			String name = found->second;

			// write component with its name and serialized values
			tempWriter.write(name, this->get_by_name(name, entity));
		}
	}
}

Node Minty::EntityRegistry::serialize_entity(Entity const entity) const
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

String Minty::to_string(EntityRegistry const& value)
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
