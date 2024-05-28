#include "pch.h"
#include "Minty/Scripting/M_ScriptComponent.h"

#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Entities/M_EntityRegistry.h"

using namespace Minty;

void Minty::ScriptEventComponent::invoke(ScriptComponent const& script) const
{
	String method = get_method_name();

	for (ID const id : scriptIds)
	{
		script.scripts.at(id).invoke(method);
	}
}

void Minty::ScriptEventComponent::invoke(ScriptComponent const& script, std::unordered_set<String> const& componentNames) const
{
	String method = get_method_name();

	for (String const& name : componentNames)
	{
		ID id = script.scripts.find(name);

		if (id == ERROR_ID) continue;

		script.scripts.at(id).invoke(method);
	}
}

void Minty::ScriptEventComponent::invoke(ScriptComponent const& script, String const& componentName) const
{
	String method = get_method_name();

	ID id = script.scripts.find(componentName);

	if (id == ERROR_ID) return;

	script.scripts.at(id).invoke(method);
}

void Minty::ScriptComponent::serialize(Writer& writer) const
{
	// write ID
	MINTY_ASSERT(writer.get_data() != nullptr);
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	ScriptEngine& scriptEngine = ScriptEngine::instance();
	
	for (auto const& [id, object] : scripts)
	{
		// write id, then write values under it
		Node node("", to_string(scriptEngine.get_id_from_script_name(object.get_class().get_name())));
		Writer scriptWriter(node, data);
		object.serialize(scriptWriter);
		writer.write(node);
	}
}

void Minty::ScriptComponent::deserialize(Reader const& reader)
{
	// read ID, get the script, load its values
	MINTY_ASSERT(reader.get_data() != nullptr);
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	ScriptEngine& scriptEngine = ScriptEngine::instance();
	EntityRegistry& registry = data->scene->get_entity_registry();

	for (Node const& node : reader.get_node().get_children())
	{
		// get id, read values
		UUID id = node.to_uuid();
		Component* component = registry.emplace_by_name(scriptEngine.get_name_from_script_id(id), data->entity);
		if (!component) continue;
		Reader scriptReader(node, data);
		component->deserialize(scriptReader);
	}
}

