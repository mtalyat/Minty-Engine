#include "pch.h"
#include "Minty/Rendering/M_Camera.h"

#include "Minty/Math/M_Math.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Tools/M_Text.h"

using namespace Minty;

String Minty::to_string(Perspective const value)
{
	switch (value)
	{
	case Perspective::Orthographic: return "ORTHOGRAPHIC";
	case Perspective::Perspective: return "PERSPECTIVE";
	default: return "";
	}
}

Perspective Minty::from_string_perspective(String const& value)
{
	String value2 = Minty::Text::to_upper(value);
	if (value2 == "ORTHOGRAPHIC") return Perspective::Orthographic;
	if (value2 == "PERSPECTIVE") return Perspective::Perspective;

	return static_cast<Perspective>(0);
}

Minty::Camera::Camera(Perspective const perspective, float const fov, float const nearPlane, float const farPlane)
	: _perspective(perspective)
	, _fov(fov * Math::DEG2RAD)
	, _near(nearPlane)
	, _far(farPlane)
	, _color(Color::black())
{}

void Minty::Camera::serialize(Writer& writer) const
{
	writer.write("perspective", to_string(_perspective));
	writer.write("fov", _fov);
	writer.write("near", _near);
	writer.write("far", _far);
	writer.write("color", _color);
}

void Minty::Camera::deserialize(Reader const& reader)
{
	String perspectiveName;
	if (reader.try_read_string("perspective", perspectiveName))
	{
		_perspective = from_string_perspective(perspectiveName);
	}
	reader.try_read_float("fov", _fov);
	reader.try_read_float("near", _near);
	reader.try_read_float("far", _far);
	reader.try_read_color("color", _color);
}
