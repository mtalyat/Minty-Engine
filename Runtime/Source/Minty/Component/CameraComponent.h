#pragma once
#include "Minty/Component/ScriptObjectComponent.h"
#include "Minty/Render/Camera.h"

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
		void deserialize(Reader& reader) override;
	};
}