#include "pch.h"
#include "M_RelationshipComponent.h"

#include "M_EntityRegistry.h"
#include "M_SerializationData.h"
#include "M_Scene.h"

using namespace minty;

void minty::RelationshipComponent::serialize(Writer& writer) const
{
	if (!writer.get_data())
	{
		Console::error("Cannot serialize RelationshipComponent. Writer data must be a Scene.");
		return;
	}

	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());

	EntityRegistry const& er = data->scene->get_entity_registry();

	writer.write("children", children);
	writer.write("first", er.get_name(first));
	writer.write("prev", er.get_name(prev));
	writer.write("next", er.get_name(next));
	writer.write("parent", er.get_name(parent));
}

void minty::RelationshipComponent::deserialize(Reader const& reader)
{
	if (!reader.get_data())
	{
		Console::error("Cannot deserialize RelationshipComponent. Reader data must be a Scene.");
		return;
	}

	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());

	EntityRegistry const& er = data->scene->get_entity_registry();

	reader.try_read_size("children", children);
	String name;
	if (reader.try_read_string("first", name)) first = er.find(name);
	if (reader.try_read_string("prev", name)) prev = er.find(name);
	if (reader.try_read_string("next", name)) next = er.find(name);
	if (reader.try_read_string("parent", name)) parent = er.find(name);
}

String minty::to_string(RelationshipComponent const& value)
{
	return std::format("RelationshipComponent(children = {}, first = {}, prev = {}, next = {}, parent = {})", std::to_string(value.children), to_string(value.first), to_string(value.prev), to_string(value.next), to_string(value.parent));
}
