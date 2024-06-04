#include "pch.h"
#include "Minty/Components/M_RelationshipComponent.h"

#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Scenes/M_Scene.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::RelationshipComponent::serialize(Writer& writer) const
{
	MINTY_ASSERT(writer.get_data() != nullptr);

	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());

	MINTY_ASSERT(data->scene != nullptr);

	EntityRegistry const& er = data->scene->get_entity_registry();

	writer.write("parent", er.get_id(parent));
}

void Minty::RelationshipComponent::deserialize(Reader const& reader)
{
	MINTY_ASSERT(reader.get_data() != nullptr);

	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());

	MINTY_ASSERT(data->scene != nullptr);

	EntityRegistry& er = data->scene->get_entity_registry();

	UUID id(INVALID_UUID);
	if (reader.try_read_uuid("parent", id))
	{
		er.set_parent(data->entity, er.find_by_id(id));
	}
}