#include "pch.h"
#include "CanvasComponent.h"

using namespace Minty;

void Minty::CanvasComponent::serialize(Writer& writer) const
{
	writer.write("resolution", referenceResolution);
}

void Minty::CanvasComponent::deserialize(Reader& reader)
{
	reader.read("resolution", referenceResolution);
}
