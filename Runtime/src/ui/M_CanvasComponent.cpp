#include "pch.h"
#include "ui/M_CanvasComponent.h"

#include "serialization/M_Writer.h"
#include "serialization/M_Reader.h"

using namespace minty;

RectF minty::CanvasComponent::toRect() const
{
	return RectF(0, 0, referenceResolutionWidth, referenceResolutionHeight);
}

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
