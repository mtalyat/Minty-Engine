#pragma once
#include "Minty/Components/M_ScriptObjectComponent.h"

#include "Minty/Rendering/M_Camera.h"

namespace Minty
{
	/// <summary>
	/// Holds data for a Camera within the Scene.
	/// </summary>
	struct CameraComponent
		: public ScriptObjectComponent
	{
		Camera camera;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}