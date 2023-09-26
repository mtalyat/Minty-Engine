#include "pch.h"
#include "M_CameraComponent.h"

#include "M_Console.h"

using namespace minty;

void minty::CameraComponent::serialize(Writer& writer) const
{
	writer.write("perspective", static_cast<byte>(perspective));
	writer.write("fov", fov);
	writer.write("near", nearPlane);
	writer.write("far", farPlane);
}

void minty::CameraComponent::deserialize(Reader const& reader)
{
	perspective = static_cast<Perspective>(reader.read_byte("perspective"));
	fov = reader.read_float("fov", DEFAULT_FOV);
	nearPlane = reader.read_float("near", DEFAULT_NEAR);
	farPlane = reader.read_float("far", DEFAULT_FAR);
}
