#pragma once

#include "M_Component.h"

namespace minty
{
	struct RotationComponent
		: public Component
	{
		// TODO: make into quaternion
		float x;
		float y;
		float z;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}