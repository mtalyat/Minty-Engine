#include "pch.h"
#include "M_CameraComponent.h"

#include "M_Console.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

void minty::CameraComponent::serialize(Writer& writer) const
{
	writer.write_as(camera);
}

void minty::CameraComponent::deserialize(Reader const& reader)
{
	reader.to_object_ref(camera);
}

String minty::to_string(CameraComponent const& value)
{
	return std::format("CameraComponent()");
}