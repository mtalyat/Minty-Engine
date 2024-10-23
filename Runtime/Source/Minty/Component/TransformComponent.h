#pragma once
#include "Minty/Component/ScriptObjectComponent.h"

#include "Minty/Core/Math.h"

namespace Minty
{
	struct TransformComponent
		: public ScriptObjectComponent
	{
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

		Matrix4 get_local_matrix() const { return glm::translate(Matrix4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(Matrix4(1.0f), localScale); }

		Float3 get_global_position() const { return matrix4_get_position(globalMatrix); }
		Quaternion get_global_rotation() const { return matrix4_get_rotation(globalMatrix); }
		Float3 get_global_scale() const { return matrix4_get_scale(globalMatrix); }

		Float3 get_forward() const { return Float3(globalMatrix * Float4(0.0f, 0.0f, 1.0f, 0.0f)); }
		Float3 get_up() const { return Float3(globalMatrix * Float4(0.0f, 1.0f, 0.0f, 0.0f)); }
		Float3 get_right() const { return Float3(globalMatrix * Float4(1.0f, 0.0f, 0.0f, 0.0f)); }

		static TransformComponent create_empty()
		{
			TransformComponent component{};
			component.globalMatrix = component.get_local_matrix();
			return component;
		}

		void serialize(Writer& writer) const override
		{
			writer.write("position", localPosition);
			writer.write("rotation", to_euler(localRotation));
			writer.write("scale", localScale);
		}

		void deserialize(Reader& reader) override
		{
			reader.read("position", localPosition);
			Float3 eulerAngles;
			if (reader.read("rotation", eulerAngles))
			{
				localRotation = from_euler(eulerAngles);
			}
			reader.read("scale", localScale);
		}
	};
}
