#include "pch.h"
#include "M_CameraComponent.h"

#include "M_Console.h"

using namespace minty;

void minty::CameraComponent::serialize(Writer& writer) const
{
	writer.write("perspective", static_cast<byte>(perspective), 0u);
	writer.write("fov", fov, DEFAULT_FOV);
	writer.write("near", nearPlane, DEFAULT_NEAR);
	writer.write("far", farPlane, DEFAULT_FAR);
}

void minty::CameraComponent::deserialize(Reader const& reader)
{
	perspective = static_cast<Perspective>(reader.read_byte("perspective"));
	fov = reader.read_float("fov", DEFAULT_FOV);
	nearPlane = reader.read_float("near", DEFAULT_NEAR);
	farPlane = reader.read_float("far", DEFAULT_FAR);
}

std::string minty::to_string(CameraComponent const& value)
{
	return std::format("CameraComponent(perspective = {}, fov = {}, near = {}, far = {})", static_cast<int>(value.perspective), value.fov, value.nearPlane, value.farPlane);
}
