#include "pch.h"
#include "M_RelationshipComponent.h"

#include "M_SerializationData.h"
#include "M_Scene.h"

void minty::RelationshipComponent::serialize(Writer& writer) const
{
	if (!writer.get_data())
	{
		console::error("Cannot serialize RelationshipComponent. Writer data must be a Scene.");
		return;
	}

	SerializationData* data = static_cast<SerializationData*>(writer.get_data());

	EntityRegistry const* er = data->scene->get_entity_registry();

	writer.write("children", children, 0ull);
	writer.write("first", er->get_name(first), "");
	writer.write("prev", er->get_name(prev), "");
	writer.write("next", er->get_name(next), "");
	writer.write("parent", er->get_name(parent), "");
}

void minty::RelationshipComponent::deserialize(Reader const& reader)
{
	if (!reader.get_data())
	{
		console::error("Cannot deserialize RelationshipComponent. Reader data must be a Scene.");
		return;
	}

	SerializationData* data = static_cast<SerializationData*>(reader.get_data());

	EntityRegistry const* er = data->scene->get_entity_registry();

	children = reader.read_size("children");
	first = er->find_by_name(reader.read_string("first"));
	prev = er->find_by_name(reader.read_string("prev"));
	next = er->find_by_name(reader.read_string("next"));
	parent = er->find_by_name(reader.read_string("parent"));
}

std::string minty::to_string(RelationshipComponent const& value)
{
	return std::format("RelationshipComponent(children = {}, first = {}, prev = {}, next = {}, parent = {})", std::to_string(value.children), to_string(value.first), to_string(value.prev), to_string(value.next), to_string(value.parent));
}
