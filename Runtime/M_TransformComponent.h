#pragma once

#include "M_Component.h"
#include "M_Transform.h"

namespace minty
{
	struct TransformComponent
		: public Component
	{
		Matrix4 global;
		Transform local;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(TransformComponent const& value);
	};
}