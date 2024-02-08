#include "pch.h"
#include "M_CameraComponent.h"

#include "M_Console.h"

using namespace minty;

void minty::CameraComponent::serialize(Writer& writer) const
{
	writer.write("perspective", to_string(perspective), to_string(Perspective::Perspective));
	writer.write("fov", fov, DEFAULT_FOV);
	writer.write("near", nearPlane, DEFAULT_NEAR);
	writer.write("far", farPlane, DEFAULT_FAR);
}

void minty::CameraComponent::deserialize(Reader const& reader)
{
	String perspectiveName;
	if (reader.try_read_string("perspective", perspectiveName))
	{
		perspective = from_string_camera_component_perspective(perspectiveName);
	}
	reader.try_read_float("fov", fov);
	reader.try_read_float("near", nearPlane);
	reader.try_read_float("far", farPlane);
}

String minty::to_string(CameraComponent const& value)
{
	return std::format("CameraComponent(perspective = {}, fov = {}, near = {}, far = {})", static_cast<int>(value.perspective), value.fov, value.nearPlane, value.farPlane);
}

String minty::to_string(CameraComponent::Perspective const value)
{
	switch (value)
	{
	case CameraComponent::Perspective::Orthographic: return "ORTHOGRAPHIC";
	case CameraComponent::Perspective::Perspective: return "PERSPECTIVE";
	default: return "";
	}
}

CameraComponent::Perspective minty::from_string_camera_component_perspective(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "ORTHOGRAPHIC") return CameraComponent::Perspective::Orthographic;
	if (value2 == "PERSPECTIVE") return CameraComponent::Perspective::Perspective;

	return static_cast<CameraComponent::Perspective>(0);
}