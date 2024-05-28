#include "pch.h"
#include "Minty/UI/M_CanvasComponent.h"

#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"

using namespace Minty;

RectF Minty::CanvasComponent::toRect() const
{
	return RectF(0, 0, referenceResolutionWidth, referenceResolutionHeight);
}

void Minty::CanvasComponent::serialize(Writer& writer) const
{
	writer.write("resolutionWidth", referenceResolutionWidth);
	writer.write("resolutionHeight", referenceResolutionHeight);
}

void Minty::CanvasComponent::deserialize(Reader const& reader)
{
	referenceResolutionWidth = reader.read_int("resolutionWidth");
	referenceResolutionHeight = reader.read_int("resolutionHeight");
}
