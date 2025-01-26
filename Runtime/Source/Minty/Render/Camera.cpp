#include "pch.h"
#include "Camera.h"

#include "Minty/Core/String.h"

using namespace Minty;

Minty::Camera::Camera(CameraBuilder const& builder)
	: m_perspective(builder.perspective)
	, m_fov(builder.fov * Math::DEG2RAD)
	, m_near(builder.nearPlane)
	, m_far(builder.farPlane)
	, m_color(builder.color)
	, m_aspectRatio(builder.aspectRatio)
	, m_size(builder.size)
	, m_layer(builder.layer)
{}

void Minty::Camera::serialize(Writer& writer) const
{
	writer.write("perspective", m_perspective);
	writer.write("fov", m_fov * Math::RAD2DEG); // stored as radians, write as degrees
	writer.write("near", m_near);
	writer.write("far", m_far);
	writer.write("color", m_color);
	writer.write("aspectRatio", m_aspectRatio);
	writer.write("size", m_size);
	writer.write("layerMask", m_layer);
}

void Minty::Camera::deserialize(Reader& reader)
{
	reader.read("perspective", m_perspective);
	// read as degrees, store as radians
	if (reader.read("fov", m_fov))
	{
		m_fov *= Math::DEG2RAD;
	}
	reader.read("near", m_near);
	reader.read("far", m_far);
	reader.read("color", m_color);
	reader.read("aspectRatio", m_aspectRatio);
	reader.read("size", m_size);
	reader.read("layerMask", m_layer);
}
