#include "pch.h"
#include "M_RelationshipComponent.h"

#include "M_EntityRegistry.h"
#include "M_SerializationData.h"
#include "M_Scene.h"

#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

void minty::RelationshipComponent::serialize(Writer& writer) const
{
	MINTY_ASSERT(writer.get_data() != nullptr);

	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());

	MINTY_ASSERT(data->scene != nullptr);

	EntityRegistry const& er = data->scene->get_entity_registry();

	writer.write("parent", er.get_id(parent));
}

void minty::RelationshipComponent::deserialize(Reader const& reader)
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

String minty::to_string(RelationshipComponent const& value)
{
	return std::format("RelationshipComponent(index = {}, children = {}, first = {}, prev = {}, next = {}, parent = {})", std::to_string(value.index), std::to_string(value.children), to_string(value.first), to_string(value.prev), to_string(value.next), to_string(value.parent));
}
