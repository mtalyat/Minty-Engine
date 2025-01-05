#include "pch.h"
#include "TagComponent.h"

using namespace Minty;

void Minty::TagComponent::serialize(Writer& writer) const
{
	writer.write("tag", tag);
}

void Minty::TagComponent::deserialize(Reader& reader)
{
	reader.read("tag", tag);
}
