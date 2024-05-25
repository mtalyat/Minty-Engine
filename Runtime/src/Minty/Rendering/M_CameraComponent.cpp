#include "pch.h"
#include "Minty/Rendering/M_CameraComponent.h"

#include "Minty/Tools/M_Console.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::CameraComponent::serialize(Writer& writer) const
{
	writer.write_as(camera);
}

void Minty::CameraComponent::deserialize(Reader const& reader)
{
	reader.to_object_ref(camera);
}