#pragma once
#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Handles the scaling of the window for the UI and all of its children.
	/// </summary>
	struct CanvasComponent
		: public Component
	{
		int referenceResolutionWidth;
		int referenceResolutionHeight;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}