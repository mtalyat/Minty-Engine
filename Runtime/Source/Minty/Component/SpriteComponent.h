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
	struct SpriteComponent
		: public Component
	{
		Ref<Sprite> sprite = nullptr;

		/// <summary>
		/// The size of the Sprite in world space.
		/// </summary>
		Float2 size = Float2(1.0f, 1.0f);

		/// <summary>
		/// The color of the Sprite.
		/// </summary>
		Color color = Color(255, 255, 255, 255);

		void serialize(Writer& writer) const override
		{
			if (sprite.get())
			{
				writer.write("sprite", sprite->id());
			}
			writer.write("size", size);
			writer.write("color", color);
		}

		void deserialize(Reader& reader) override
		{
			UUID temp;
			if (reader.read("sprite", temp))
			{
				sprite = AssetManager::get<Sprite>(temp);
			}
			reader.read("size", size);
			reader.read("color", color);
		}
	};
}
