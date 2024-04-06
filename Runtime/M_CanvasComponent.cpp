#include "pch.h"
#include "M_CanvasComponent.h"

#include "M_Writer.h"
#include "M_Reader.h"

using namespace minty;

void minty::CanvasComponent::serialize(Writer& writer) const
{
	writer.write("resolutionWidth", referenceResolutionWidth);
	writer.write("resolutionHeight", referenceResolutionHeight);
}

void minty::CanvasComponent::deserialize(Reader const& reader)
{
	referenceResolutionWidth = reader.read_int("resolutionWidth");
	referenceResolutionHeight = reader.read_int("resolutionHeight");
}
