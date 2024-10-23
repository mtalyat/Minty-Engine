#include "pch.h"
#include "CameraComponent.h"

using namespace Minty;

void Minty::CameraComponent::serialize(Writer& writer) const
{
	camera.serialize(writer);
}

void Minty::CameraComponent::deserialize(Reader& reader)
{
	camera.deserialize(reader);
}
