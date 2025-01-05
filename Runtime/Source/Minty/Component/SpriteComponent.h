#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Color.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"
#include "Minty/Render/Sprite.h"

namespace Minty
{
	/// <summary>
	/// Holds a Sprite for this Entity.
	/// </summary>
	struct SpriteComponent
		: public Component
	{
		/// <summary>
		/// A reference to the Sprite.
		/// </summary>
		Ref<Sprite> sprite = nullptr;

		/// <summary>
		/// The color of the Sprite.
		/// </summary>
		Color color = Color(255, 255, 255, 255);

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}
