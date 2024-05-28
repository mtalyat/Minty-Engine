#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Types/M_RectF.h"

namespace Minty
{
	/// <summary>
	/// Handles the scaling of the window for the UI and all of its children.
	/// </summary>
	struct CanvasComponent
		: public Component
	{
		int referenceResolutionWidth;
		int referenceResolutionHeight;

		RectF toRect() const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}