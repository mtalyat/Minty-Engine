#pragma once

#include "M_Component.h"
#include "M_Vector.h"

namespace minty
{
	struct OriginComponent
		: public Component
	{
		Vector3 position;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(OriginComponent const& value);
	};
}