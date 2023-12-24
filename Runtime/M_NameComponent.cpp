#include "pch.h"
#include "M_NameComponent.h"

using namespace minty;

void minty::NameComponent::serialize(Writer& writer) const
{
	writer.write("name", name);
}

void minty::NameComponent::deserialize(Reader const& reader)
{
	name = reader.read_string("name");
}

String minty::to_string(NameComponent const& value)
{
	return std::format("NameComponent(name = {})", value.name);
}
