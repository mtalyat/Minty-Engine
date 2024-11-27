#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/Math.h"
#include "Minty/Core/Rect.h"

namespace Minty
{
	struct CanvasComponent
		: public Component
	{
		/// <summary>
		/// The simulated resolution that the children of this Canvas use to position themselves with.
		/// </summary>
		Int2 referenceResolution;

		/// <summary>
		/// Gets the Rect of this Canvas.
		/// </summary>
		/// <returns></returns>
		Rect to_rect() const { return Rect(Float2(), static_cast<Float2>(referenceResolution)); }

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}
