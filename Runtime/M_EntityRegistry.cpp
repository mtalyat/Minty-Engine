#include "pch.h"
#include "M_EntityRegistry.h"

#include "M_SerializationData.h"
#include "M_Console.h"
#include "M_Runtime.h"

#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_TransformComponent.h"
#include "M_DirtyComponent.h"
#include "M_DestroyComponent.h"

#include "M_Script.h"
#include "M_ScriptObject.h"
#include "M_ScriptEngine.h"
#include "M_ScriptComponent.h"

#include <sstream>
#include <map>

using namespace minty;

std::map<String const, EntityRegistry::ComponentFuncs const> EntityRegistry::_components = std::map<String const, EntityRegistry::ComponentFuncs const>();
std::map<uint32_t const, String const> EntityRegistry::_componentTypes = std::map<uint32_t const, String const>();

minty::EntityRegistry::EntityRegistry(Runtime& engine, ID const sceneId)
	: SceneObject(engine, sceneId)
	, entt::registry()
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
	return entt::registry::create();
}

Entity minty::EntityRegistry::create(String const& name)
{
	Entity e = create();
	NameComponent& nameComponent = this->emplace<NameComponent>(e);
	nameComponent.name = name;
	return e;
}

void minty::EntityRegistry::destroy(Entity const entity)
{
	emplace_or_replace<DestroyComponent>(entity);
}

void minty::EntityRegistry::destroy_immediate(Entity const entity)
{
	// if has an OnDestroy script, call that
	ScriptComponent const* script = try_get<ScriptComponent>(entity);
	ScriptOnDestroyComponent const* ondestroy = try_get<ScriptOnDestroyComponent>(entity);
	if (script && ondestroy)
	{
		ondestroy->invoke(SCRIPT_METHOD_NAME_ONDESTROY, *script);
	}

	// destroy entity
	entt::registry::destroy(entity);
}

void minty::EntityRegistry::destroy_all()
{
	// call OnDestroy on any scripts
	for (auto [entity, destroy, script, ondestroy] : view<DestroyComponent const, ScriptComponent const, ScriptOnDestroyComponent const>().each())
	{
		ondestroy.invoke(SCRIPT_METHOD_NAME_ONDESTROY, script);
	}

	// get the entities that are ready for destruction
	auto destroyView = view<DestroyComponent const>();

	// destroy them all
	entt::registry::destroy(destroyView.begin(), destroyView.end());
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
}

Entity minty::EntityRegistry::find(String const& string) const
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

void minty::EntityRegistry::set_name(Entity const entity, String const& name)
{
	// check if name is a real name, or empty
	if (name.empty() || (name.size() == 1 && name.front() == '_'))
	{
		// name is empty, remove the component if it exists
		if(any_of<NameComponent>(entity))
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
			else if(component->scripts.contains(name))
			{
				// if the script component exists, do nothing
				return component;
			}

			// get the script engine
			ScriptEngine& engine = registry.get_runtime().get_script_engine();

			// get the script based on the name
			Script const* script = engine.get_script(name);

			// no script found
			if (!script) return nullptr;

			// add a script object to it
			ID id = component->scripts.emplace(name, ScriptObject(*script));
			component->scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONCREATE);

			// now add the helper components, if they are needed
			if (script->has_method(SCRIPT_METHOD_NAME_ONLOAD))
			{
				ScriptOnLoadComponent* eventComp = registry.try_get<ScriptOnLoadComponent>(entity);
				if (!eventComp) eventComp = &registry.emplace<ScriptOnLoadComponent>(entity);
				eventComp->scriptIds.emplace(id);
			}
			if (script->has_method(SCRIPT_METHOD_NAME_ONUPDATE))
			{
				ScriptOnUpdateComponent* eventComp = registry.try_get<ScriptOnUpdateComponent>(entity);
				if (!eventComp) eventComp = &registry.emplace<ScriptOnUpdateComponent>(entity);
				eventComp->scriptIds.emplace(id);
			}
			if (script->has_method(SCRIPT_METHOD_NAME_ONUNLOAD))
			{
				ScriptOnUnloadComponent* eventComp = registry.try_get<ScriptOnUnloadComponent>(entity);
				if (!eventComp) eventComp = &registry.emplace<ScriptOnUnloadComponent>(entity);
				eventComp->scriptIds.emplace(id);
			}
			if (script->has_method(SCRIPT_METHOD_NAME_ONDESTROY))
			{
				ScriptOnDestroyComponent* eventComp = registry.try_get<ScriptOnDestroyComponent>(entity);
				if (!eventComp) eventComp = &registry.emplace<ScriptOnDestroyComponent>(entity);
				eventComp->scriptIds.emplace(id);
			}

			// return the component
			return component;
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

			// found it
			return &component->scripts.at(found);
		},
		.erase = [](EntityRegistry& registry, Entity const entity, String const& name) -> void
		{
			// get the script component
			ScriptComponent* component = registry.try_get<ScriptComponent>(entity);

			// get the ID of the script with the name
			ID id = component->scripts.get_id(name);

			// if not there, skip
			if (id == ERROR_ID) return;

			// destroy the script
			component->scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONDESTROY);
			component->scripts.erase(name);

			// get the other components and erase if needed
			if (ScriptOnLoadComponent* eventComp = registry.try_get<ScriptOnLoadComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnUpdateComponent* eventComp = registry.try_get<ScriptOnUpdateComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnUnloadComponent* eventComp = registry.try_get<ScriptOnUnloadComponent>(entity)) eventComp->scriptIds.erase(id);
			if (ScriptOnDestroyComponent* eventComp = registry.try_get<ScriptOnDestroyComponent>(entity)) eventComp->scriptIds.erase(id);
		},
	};
	_components.emplace(name, funcs);

	Console::info(std::format("Registered script {}.", name));
}

void minty::EntityRegistry::serialize(Writer& writer) const
{
	// write each entity, and each component under it
	String entityName;

	for (auto [entity] : this->storage<Entity>()->each())
	{
		// get name
		entityName = this->get_name(entity);

		// use "-" instead of ""
		if (entityName.empty()) entityName = "-";

		// serialize entity
		Node entityNode(entityName);
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

	// for each entity name given
	for (auto const& entityNode : node.get_children())
	{
		// create the entity
		Entity entity = create();
		data.entity = entity;

		// set name
		set_name(entity, entityNode.get_name());

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

			// ignore NameComponent, that is used when writing the entity node
			if (name.compare("Name") == 0)
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
	Node node;
	Writer writer(node);
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
