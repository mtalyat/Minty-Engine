#include "pch.h"
#include "M_NameComponent.h"

void minty::NameComponent::serialize(Writer& writer) const
{
	writer.write("name", name);
}

void minty::NameComponent::deserialize(Reader const& reader)
{
	name = reader.read_string("name");
}