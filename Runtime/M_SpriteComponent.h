#pragma once

#include "M_Component.h"
#include "M_Transform.h"

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
		ID spriteId;

		/// <summary>
		/// The size of the Sprite in world space.
		/// </summary>
		Vector2 size;

		/// <summary>
		/// The order in which to render the Sprite, when it has equal distance to the Camera as another Sprite.
		/// </summary>
		int order;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(SpriteComponent const& value);
	};
}