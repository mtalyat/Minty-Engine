#pragma once

#include "M_Component.h"
#include "M_Vector3.h"

namespace minty
{
	struct PositionComponent
		: public Component
	{
		Vector3 position;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}