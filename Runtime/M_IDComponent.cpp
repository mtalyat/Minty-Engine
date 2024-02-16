#include "pch.h"
#include "M_IDComponent.h"

void minty::IDComponent::serialize(Writer& writer) const
{
	writer.get_node().set_data(to_string(id));
}

void minty::IDComponent::deserialize(Reader const& reader)
{
	id = reader.to_uuid();
}
