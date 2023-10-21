#pragma once

#include "M_Component.h"
#include "M_Transform.h"

namespace minty
{
	enum class AnchorMode : int
	{
		Top =		0b00000001,
		Middle =	0b00000010,
		Bottom =	0b00000100,
		Left =		0b00001000,
		Center =	0b00010000,
		Right =		0b00100000,
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