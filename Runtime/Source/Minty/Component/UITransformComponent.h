#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/Rect.h"
#include "Minty/Entity/Entity.h"
#include "Minty/UI/AnchorMode.h"

namespace Minty
{
	struct UITransformComponent
		: public Component
	{
		Entity canvas;

		AnchorMode anchorMode;

		union { Float x, left; };
		union { Float y, top; };
		union { Float z, depth; };

		union { Float width, right; };
		union { Float height, bottom; };

		Rect globalRect;

		void update_global_rect(Rect const parentRect);

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}
