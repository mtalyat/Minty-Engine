#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Color.h"
#include "Minty/Render/Perspective.h"
#include "Minty/Serialize/Serializable.h"
#include "Minty/Render/RenderTarget.h"

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
		Layer layer = LAYER_ALL;
		Ref<RenderTarget> renderTarget = nullptr;
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
		Layer m_layer;
		Ref<RenderTarget> m_renderTarget;

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

		Layer get_layer_mask() const { return m_layer; }

		Ref<RenderTarget>& get_render_target() { return m_renderTarget; }

		Ref<RenderTarget> const& get_render_target() const { return m_renderTarget; }

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

		void set_layer_mask(Layer const mask) { m_layer = mask; }

		void set_render_target(Ref<RenderTarget> const& renderTarget) { m_renderTarget = renderTarget; }

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}