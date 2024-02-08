#pragma once

#include "M_Component.h"
#include "M_Matrix.h"
#include "M_Vector.h"
#include "M_Quaternion.h"

namespace minty
{
	struct TransformComponent
		: public Component
	{
		Matrix4 globalMatrix;

		/// <summary>
		/// The position.
		/// </summary>
		Vector3 localPosition;

		/// <summary>
		/// The rotation.
		/// </summary>
		Quaternion localRotation;

		/// <summary>
		/// The scale.
		/// </summary>
		Vector3 localScale = Vector3(1.0f, 1.0f, 1.0f);

		Matrix4 get_local_matrix() const;

		Vector3 get_global_position() const;

		Vector3 get_forward() const;
		Vector3 get_up() const;
		Vector3 get_right() const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(TransformComponent const& value);
	};
}