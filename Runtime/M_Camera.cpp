#include "pch.h"
#include "M_Camera.h"

#include "M_Math.h"
#include "M_Writer.h"
#include "M_Reader.h"
#include "M_Text.h"

using namespace minty;

String minty::to_string(Perspective const value)
{
	switch (value)
	{
	case Perspective::Orthographic: return "ORTHOGRAPHIC";
	case Perspective::Perspective: return "PERSPECTIVE";
	default: return "";
	}
}

Perspective minty::from_string_perspective(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "ORTHOGRAPHIC") return Perspective::Orthographic;
	if (value2 == "PERSPECTIVE") return Perspective::Perspective;

	return static_cast<Perspective>(0);
}

minty::Camera::Camera(Perspective const perspective, float const fov, float const nearPlane, float const farPlane)
	: _perspective(perspective)
	, _fov(fov * Math::DEG2RAD)
	, _near(nearPlane)
	, _far(farPlane)
	, _color(Color::black())
{}

Perspective minty::Camera::get_perspective() const
{
	return _perspective;
}

float minty::Camera::get_fov() const
{
	return _fov * Math::RAD2DEG;
}

float minty::Camera::get_near() const
{
	return _near;
}

float minty::Camera::get_far() const
{
	return _far;
}

Color minty::Camera::get_color() const
{
	return _color;
}

void minty::Camera::set_perspective(Perspective const perspective)
{
	_perspective = perspective;
}

void minty::Camera::set_fov(float const fov)
{
	_fov = fov * Math::DEG2RAD;
}

void minty::Camera::set_near(float const nearPlane)
{
	_near = nearPlane;
}

void minty::Camera::set_far(float const farPlane)
{
	_far = farPlane;
}

void minty::Camera::set_color(Color const color)
{
	_color = color;
}

void minty::Camera::serialize(Writer& writer) const
{
	writer.write("perspective", to_string(_perspective));
	writer.write("fov", _fov);
	writer.write("near", _near);
	writer.write("far", _far);
	writer.write("color", _color);
}

void minty::Camera::deserialize(Reader const& reader)
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
