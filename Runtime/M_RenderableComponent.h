#pragma once

#include "M_Component.h"

namespace minty
{
	struct RenderableComponent
		: public Component
	{
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(RenderableComponent const& value);
	};
}