#pragma once

#include "M_Component.h"

namespace minty
{
	struct Dirty
		: public Component
	{
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(Dirty const& value);
	};
}