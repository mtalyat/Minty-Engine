#pragma once
#include "M_Component.h"

namespace minty
{
	struct TagComponent
		: public Component
	{
		String tag;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}