#pragma once
#include "Minty/Component/ScriptObjectComponent.h"

#include "Minty/Core/Math.h"

namespace Minty
{
	/// <summary>
	/// Holds 3D space transform data for an Entity.
	/// </summary>
	struct TransformComponent
		: public ScriptObjectComponent
	{
		/// <summary>
		/// The matrix containing the global transform data.
		/// </summary>
		Matrix4 globalMatrix{};

		/// <summary>
		/// The position.
		/// </summary>
		Float3 localPosition = Float3(0.0f, 0.0f, 0.0f);

		/// <summary>
		/// The rotation.
		/// </summary>
		Quaternion localRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

		/// <summary>
		/// The scale.
		/// </summary>
		Float3 localScale = Float3(1.0f, 1.0f, 1.0f);

		/// <summary>
		/// Gets the local transform matrix.
		/// </summary>
		/// <returns></returns>
		Matrix4 get_local_matrix() const { return glm::translate(Matrix4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(Matrix4(1.0f), localScale); }

		Float3 get_global_position() const { return matrix4_get_position(globalMatrix); }
		Quaternion get_global_rotation() const { return matrix4_get_rotation(globalMatrix); }
		Float3 get_global_scale() const { return matrix4_get_scale(globalMatrix); }

		Float3 get_forward() const { return Float3(globalMatrix * Float4(0.0f, 0.0f, 1.0f, 0.0f)); }
		Float3 get_up() const { return Float3(globalMatrix * Float4(0.0f, 1.0f, 0.0f, 0.0f)); }
		Float3 get_right() const { return Float3(globalMatrix * Float4(1.0f, 0.0f, 0.0f, 0.0f)); }

		static TransformComponent create_empty();

		void serialize(Writer& writer) const override;

		void deserialize(Reader& reader) override;
	};
}
