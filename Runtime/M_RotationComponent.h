#pragma once

#include "M_Component.h"
#include "M_Vector.h"

namespace minty
{
	struct RotationComponent
		: public Component
	{
		// TODO: make into quaternion
		Vector3 rotation;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}