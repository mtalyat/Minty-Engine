#pragma once
#include "M_Component.h"

#include "M_Camera.h"

namespace minty
{
	/// <summary>
	/// Holds data for a Camera within the Scene.
	/// </summary>
	struct CameraComponent
		: public Component
	{
		Camera camera;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(CameraComponent const& value);
	};
}