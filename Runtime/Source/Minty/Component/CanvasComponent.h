#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/Math.h"
#include "Minty/Core/Rect.h"

namespace Minty
{
	struct CanvasComponent
		: public Component
	{
		Int2 referenceResolution;

		Rect to_rect() const { return Rect(Float2(), static_cast<Float2>(referenceResolution)); }

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}
