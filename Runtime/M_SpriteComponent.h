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
		ID id;

		/// <summary>
		/// The layer on which to render the Sprite.
		/// </summary>
		int layer;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(SpriteComponent const& value);
	};
}