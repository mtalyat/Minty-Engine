#pragma once

#include "M_Base.h"
#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Holds data for a Camera within the Scene.
	/// </summary>
	struct CameraComponent
		: public Component
	{
	private:
		float const DEFAULT_FOV = 45.0f;
		float const DEFAULT_NEAR = 0.1f;
		float const DEFAULT_FAR = 100.0f;

	public:
		enum class Perspective : Byte
		{
			Orthographic,
			Perspective,
		};

		/// <summary>
		/// The perspective of this Camera.
		/// </summary>
		Perspective perspective = Perspective::Perspective;

		/// <summary>
		/// The Field Of View of this Camera.
		/// </summary>
		float fov = DEFAULT_FOV;

		/// <summary>
		/// The near plane of this Camera. All renderered objects within this distance to this Camera will be clipped.
		/// </summary>
		float nearPlane = DEFAULT_NEAR;
		/// <summary>
		/// The far plane of this Camera. All renderered objects outside of this distance from this Camera will be clipped.
		/// </summary>
		float farPlane = DEFAULT_FAR;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(CameraComponent const& value);
	};

	String to_string(CameraComponent::Perspective const value);
	CameraComponent::Perspective from_string_camera_component_perspective(String const& value);
}