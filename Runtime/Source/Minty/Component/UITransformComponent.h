#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Entity/Entity.h"
#include "Minty/UI/AnchorMode.h"

namespace Minty
{
	struct UITransformComponent
		: public Component
	{
		Entity canvas;

		AnchorMode anchorMode;

		union { float x, left; };
		union { float y, top; };
		union { float z, depth; };

		union { float width, right; };
		union { float height, bottom; };

		//RectF globalRect;

		void serialize(Writer& writer) const override
		{
			writer.write("anchor", anchorMode);
			
			if ((anchorMode & (AnchorMode::Left | AnchorMode::Center | AnchorMode::Right)) == AnchorMode::All)
			{
				// stretching horizontally
				writer.write("left", left);
				writer.write("right", right);
			}
			else
			{
				// fixed horizontally
				writer.write("x", x);
				writer.write("width", width);
			}

			if ((anchorMode & (AnchorMode::Top | AnchorMode::Middle | AnchorMode::Bottom)) == AnchorMode::All)
			{
				// stretching vertically
				writer.write("top", top);
				writer.write("bottom", bottom);
			}
			else
			{
				// fixed vertically
				writer.write("y", y);
				writer.write("height", height);
			}

			writer.write("z", z);
		}

		void deserialize(Reader& reader) override
		{
			reader.read("anchor", anchorMode);

			if(!reader.read("x", x))
				reader.read("left", left);
			if (!reader.read("y", y))
				reader.read("top", top);
			reader.read("z", z);

			if (!reader.read("width", width))
				reader.read("right", right);
			if (!reader.read("height", height))
				reader.read("bottom", bottom);
		}
	};
}
