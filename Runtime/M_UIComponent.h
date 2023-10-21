#pragma once

#include "M_Component.h"
#include "M_Transform.h"

namespace minty
{
	enum class AnchorMode : byte
	{
		// anchor to top left
		TopLeft,
		// anchor to top center
		TopCenter,
		// anchor to top right
		TopRight,
		// anchor to middle left
		MiddleLeft,
		// anchor to middle center
		MiddleCenter,
		// anchor to middle right
		MiddleRight,
		// anchor to bottom left
		BottomLeft,
		// anchor to bottom center
		BottomCenter,
		// anchor to bottom right
		BottomRight,
		// stretch left/right, anchor to top
		Top,
		// stretch left/right, anchor to middle
		Middle,
		// stretch left/right, anchor to bottom
		Bottom,
		// stretch top/bottom, anchor to left
		Left,
		// stretch top/bottom, anchor to center
		Center,
		// stretch top/bottom, anchor to right
		Right,
		// stretch both
		All
	};

	struct UIComponent
		: public Component
	{
		AnchorMode anchorMode;

		union { float x, left; };
		union { float y, top; };
		union { float width, right; };
		union { float height, bottom; };

		int layer;

		ID spriteId;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(UIComponent const& value);
	};
}