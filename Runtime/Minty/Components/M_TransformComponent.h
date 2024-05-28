#pragma once
#include "Minty/Components/M_ScriptObjectComponent.h"

#include "Minty/Types/M_Matrix.h"
#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_Quaternion.h"

namespace Minty
{
	/// <summary>
	/// Holds the Entity's positional data.
	/// </summary>
	struct TransformComponent
		: public ScriptObjectComponent
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
		Quaternion get_global_rotation() const;
		Vector3 get_global_scale() const;

		Vector3 get_forward() const;
		Vector3 get_up() const;
		Vector3 get_right() const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}