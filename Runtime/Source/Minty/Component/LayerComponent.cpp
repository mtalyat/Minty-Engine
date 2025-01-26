#include "pch.h"
#include "LayerComponent.h"

void Minty::LayerComponent::serialize(Writer& writer) const
{
	writer.write("layer", layer);
}

void Minty::LayerComponent::deserialize(Reader& reader)
{
	reader.read("layer", layer);
}
