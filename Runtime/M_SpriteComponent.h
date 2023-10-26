#pragma once

#include "M_Component.h"
#include "M_Transform.h"

namespace minty
{
	struct SpriteComponent
		: public Component
	{
		ID id;
		int layer;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(SpriteComponent const& value);
	};
}