#include "pch.h"
#include "ScriptComponent.h"

#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Entity/EntitySerializationData.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Script/ScriptClass.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/Script/ScriptObject.h"
#include <unordered_map>
#include <vector>

using namespace Minty;

void Minty::ScriptComponent::serialize(Writer& writer) const
{
	// write each object under its class ID, in case name changes
	Ref<ScriptClass> scriptClass;
	String idString;
	for (auto const& [id, name, value] : scriptObjects)
	{
		scriptClass = value->get_class();
		idString = to_string(scriptClass->id());

		writer.write(idString, *value.get());
	}
}

void Minty::ScriptComponent::deserialize(Reader& reader)
{
	EntitySerializationData* data = static_cast<EntitySerializationData*>(reader.get_data());

	MINTY_ASSERT(data != nullptr);

	EntityRegistry& entityRegistry = data->scene->get_entity_registry();

	String idString;
	UUID id;
	Ref<ScriptClass> scriptClass;
	Component* component;
	ScriptObject* scriptObject;
	for (Size i = 0; i < reader.size(); i++)
	{
		reader.read_name(i, idString);
		id = Parse::to_uuid(idString);

		MINTY_ASSERT_MESSAGE(id.valid(), "Invalid ID.");

		// get class with the id
		scriptClass = ScriptEngine::find_class(id);

		MINTY_ASSERT_FORMAT(scriptClass != nullptr, "No script found with ID \"{}\" ({}).", idString, to_string(id));

		String name = scriptClass->get_full_name();

		// create object
		component = entityRegistry.emplace_by_name(name, data->entity);
		scriptObject = static_cast<ScriptObject*>(component);
		//scriptObject = ScriptEngine::create_object(UUID::create(), scriptClass);

		// read data
		reader.read(i, *scriptObject);

		// save
		scriptObjects.emplace(id, name, scriptObject->create_ref());
	}
}
