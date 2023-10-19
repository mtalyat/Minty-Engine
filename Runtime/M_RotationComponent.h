#pragma once

#include "M_Component.h"
#include "M_Vector.h"
#include "M_Quaternion.h"

namespace minty
{
	struct RotationComponent
		: public Component
	{
		Quaternion rotation;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(RotationComponent const& value);
	};
}