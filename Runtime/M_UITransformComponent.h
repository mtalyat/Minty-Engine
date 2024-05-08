#pragma once

#include "M_Entity.h"
#include "M_Component.h"
#include "M_RectF.h"

namespace minty
{
	enum class AnchorMode : int
	{
		Empty = 0,
		All = 0b00000000,
		Top = 0b00000001,
		Middle = 0b00000010,
		Bottom = 0b00000100,
		Left = 0b00001000,
		Center = 0b00010000,
		Right = 0b00100000,
	};

	String to_string(AnchorMode const anchor);
	AnchorMode operator |(AnchorMode const left, AnchorMode const right);
	AnchorMode operator &(AnchorMode const left, AnchorMode const right);

	AnchorMode from_string_anchor_mode(String const& string);

	struct UITransformComponent
		: public Component
	{
		Entity canvas;

		AnchorMode anchorMode;

		union { float x, left; };
		union { float y, top; };
		union { float width, right; };
		union { float height, bottom; };

		RectF globalRect;

		void update_global_rect(RectF const parentRect);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(UITransformComponent const& value);
	};
}