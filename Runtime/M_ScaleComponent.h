#pragma once

#include "M_Component.h"
#include "M_Vector.h"

namespace minty
{
	struct ScaleComponent
		: public Component
	{
		Vector3 scale;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}