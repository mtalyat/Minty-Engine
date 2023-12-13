#pragma once

#include "M_Component.h"

namespace minty
{
	struct AudioListener
		: public Component
	{
		void serialize(Writer& writer) const override {}
		void deserialize(Reader const& reader) override {}
	};
}