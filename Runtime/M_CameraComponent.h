#pragma once

#include "M_Component.h"

namespace minty
{
	struct CameraComponent
		: public Component
	{
	private:
		float const DEFAULT_FOV = 45.0f;
		float const DEFAULT_NEAR = 0.1f;
		float const DEFAULT_FAR = 100.0f;

	public:

		enum class Perspective : byte
		{
			Orthographic,
			Perspective,
		};

		Perspective perspective = Perspective::Perspective;

		float fov = DEFAULT_FOV;

		float nearPlane = DEFAULT_NEAR;
		float farPlane = DEFAULT_FAR;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}