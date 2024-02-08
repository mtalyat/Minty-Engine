#pragma once
#include "M_Base.h"

#include "M_Color.h"
#include "M_Rendering_PixelFormat.h"
#include "M_CoordinateMode.h"
#include "M_Vector.h"
#include "vulkan.h"

namespace minty
{
	/// <summary>
	/// Holds data to create a new Sprite.
	/// </summary>
	struct SpriteBuilder
	{
		/// <summary>
		/// The name of the Sprite.
		/// </summary>
		std::string name;

		/// <summary>
		/// The ID of the Texture that is being used.
		/// </summary>
		ID textureId = ERROR_ID;

		/// <summary>
		/// The ID of the Material to use.
		/// </summary>
		ID materialId = ERROR_ID;

		/// <summary>
		/// The type of input of the data within minimum, maximum and pivot.
		/// </summary>
		CoordinateMode coordinateMode = CoordinateMode::Normalized;

		/// <summary>
		/// The minimum position of the Sprite within the Texture.
		/// </summary>
		Vector2 minCoords = Vector2(0.0f, 0.0f);

		/// <summary>
		/// The maximum position of the Sprite within the Texture.
		/// </summary>
		Vector2 maxCoords = Vector2(1.0f, 1.0f);

		/// <summary>
		/// The offset to the center of the Sprite when rendered.
		/// </summary>
		Vector2 pivot = Vector2(0.5f, 0.5f);
	};
}

