#include "pch.h"
#include "NameComponent.h"

using namespace Minty;

void Minty::NameComponent::serialize(Writer& writer) const
{
	writer.write("name", name);
}

void Minty::NameComponent::deserialize(Reader& reader)
{
	reader.read("name", name);
}
