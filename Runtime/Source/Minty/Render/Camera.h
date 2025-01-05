#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Color.h"
#include "Minty/Render/Perspective.h"
#include "Minty/Serialize/Serializable.h"

namespace Minty
{
	struct CameraBuilder
	{
		Perspective perspective = Perspective::Perspective;
		Float fov = 45.0f;
		Float nearPlane = 0.1f;
		Float farPlane = 100.0f;
		Color color = Color::black();
		Float aspectRatio = 16.0f / 9.0f;
		Float size = 5.0f;
	};

	class Camera
		: public Serializable
	{
	private:
		Perspective m_perspective;
		Float m_fov;
		Float m_near;
		Float m_far;
		Color m_color;
		Float m_aspectRatio;
		Float m_size;

	public:
		Camera(CameraBuilder const& builder = {});

#pragma region Get

	public:
		Perspective get_perspective() const { return m_perspective; }

		Float get_fov() const { return m_fov * Math::RAD2DEG; }

		Float get_near() const { return m_near; }

		Float get_far() const { return m_far; }

		Color get_color() const { return m_color; }

		Float get_aspect_ratio() const { return m_aspectRatio; }

		Float get_size() const { return m_size; }

#pragma endregion

#pragma region Set

	public:
		void set_perspective(Perspective const perspective) { m_perspective = perspective; }

		void set_fov(Float const fov) { m_fov = fov * Math::DEG2RAD; }

		void set_near(Float const nearPlane) { m_near = nearPlane; }

		void set_far(Float const farPlane) { m_far = farPlane; }

		void set_color(Color const color) { m_color = color; }

		void set_aspect_ratio(Float const aspectRatio) { m_aspectRatio = aspectRatio; }

		void set_size(Float const size) { m_size = size; }

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}