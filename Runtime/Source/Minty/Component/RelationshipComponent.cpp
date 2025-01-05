#include "pch.h"
#include "RelationshipComponent.h"

using namespace Minty;

void Minty::RelationshipComponent::serialize(Writer& writer) const
{
	EntitySerializationData* data = static_cast<EntitySerializationData*>(writer.get_data());

	if (data && data->scene)
	{
		EntityRegistry& entityRegistry = data->scene->get_entity_registry();

		writer.write("parent", entityRegistry.get_id(parent));
	}
}

void Minty::RelationshipComponent::deserialize(Reader& reader)
{
	EntitySerializationData* data = static_cast<EntitySerializationData*>(reader.get_data());

	if (data && data->scene)
	{
		EntityRegistry& entityRegistry = data->scene->get_entity_registry();

		UUID parentId;
		if (reader.read("parent", parentId))
		{
			// set parent
			entityRegistry.set_parent(data->entity, entityRegistry.find_by_id(parentId));
		}
	}
}
