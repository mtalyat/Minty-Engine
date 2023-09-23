#pragma once

#include "M_Component.h"

namespace minty
{
	struct CameraComponent
		: public Component
	{
		enum class Perspective : byte
		{
			Orthographic,
			Perspective,
		};

		Perspective perspective = Perspective::Perspective;

		float fov = 45.0f;

		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}