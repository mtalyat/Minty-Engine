#pragma once

#include "Minty/Components/M_Component.h"
#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_Color.h"

namespace Minty
{
	class Sprite;

	/// <summary>
	/// Holds data for a Sprite within the Scene.
	/// </summary>
	struct SpriteComponent
		: public Component
	{
		Ref<Sprite> sprite;

		/// <summary>
		/// The layer in which the Sprite exists. Used for sorting and rendering.
		/// </summary>
		int layer = 0;

		/// <summary>
		/// The order within the layer in which to render the Sprite.
		/// </summary>
		int order = 0;

		/// <summary>
		/// The size of the Sprite in world space.
		/// </summary>
		Vector2 size = Vector2(1.0f, 1.0f);

		/// <summary>
		/// The color of the Sprite.
		/// </summary>
		Color color = Color(255, 255, 255, 255);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}