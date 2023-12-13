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

		Vector3 get_global_position() const;

		Vector3 get_forward() const;
		Vector3 get_up() const;
		Vector3 get_right() const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(TransformComponent const& value);
	};
}