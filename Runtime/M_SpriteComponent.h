#pragma once

#include "M_Component.h"
#include "M_Vector.h"

namespace minty
{
	/// <summary>
	/// Holds data for a Sprite within the Scene.
	/// </summary>
	struct SpriteComponent
		: public Component
	{
		/// <summary>
		/// The ID of the Sprite to render.
		/// </summary>
		ID spriteId = ERROR_ID;

		/// <summary>
		/// The order in which to render the Sprite, when it has equal distance to the Camera as another Sprite.
		/// </summary>
		int order = 0;

		/// <summary>
		/// The size of the Sprite in world space.
		/// </summary>
		Vector2 size = Vector2(1.0f, 1.0f);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(SpriteComponent const& value);
	};
}