#include "pch.h"
#include "M_ScriptComponent.h"

#include "M_SerializationData.h"
#include "M_Writer.h"
#include "M_Reader.h"
#include "M_Runtime.h"
#include "M_Scene.h"
#include "M_ScriptEngine.h"

using namespace minty;

void minty::ScriptEventComponent::invoke(String const& method, ScriptComponent const& script) const
{
	for (ID const id : scriptIds)
	{
		script.scripts.at(id).invoke(method);
	}
}

void minty::ScriptEventComponent::invoke(String const& method, ScriptComponent const& script, std::unordered_set<String> const& componentNames) const
{
	for (String const& name : componentNames)
	{
		ID id = script.scripts.find(name);

		if (id == ERROR_ID) continue;

		script.scripts.at(id).invoke(method);
	}
}

void minty::ScriptComponent::serialize(Writer& writer) const
{
	// write ID
	MINTY_ASSERT(writer.get_data() != nullptr);
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	ScriptEngine& scriptEngine = data->scene->get_runtime().get_script_engine();
	
	for (auto const& [id, object] : scripts)
	{
		// write id, then write values under it
		Node node("", to_string(scriptEngine.get_id_from_script_name(object.get_class().get_name())));
		Writer scriptWriter(node, data);
		object.serialize(scriptWriter);
	}
}

void minty::ScriptComponent::deserialize(Reader const& reader)
{
	// read ID, get the script, load its values
	MINTY_ASSERT(reader.get_data() != nullptr);
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	ScriptEngine& scriptEngine = data->scene->get_runtime().get_script_engine();
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
