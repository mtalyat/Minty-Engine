#pragma once

#include "M_Base.h"
#include "M_CoordinateMode.h"
#include "M_Vector.h"

namespace minty::rendering
{
	/// <summary>
	/// Holds data to create a new TextureAtlas.
	/// </summary>
	struct TextureAtlasBuilder
	{
		/// <summary>
		/// The ID of the Texture to be used.
		/// </summary>
		ID textureId = ERROR_ID;

		/// <summary>
		/// The ID of the Material to be used. If left as ERROR_ID, a new Material will be generated using the texture.
		/// </summary>
		ID materialId = ERROR_ID;

		/// <summary>
		/// The coordinate mode for the pivot and slice data.
		/// </summary>
		CoordinateMode coordinateMode = CoordinateMode::Normalized;

		/// <summary>
		/// The size of a slice within the atlas.
		/// </summary>
		Vector2 slice = Vector2(1.0f, 1.0f);

		/// <summary>
		/// The location of the pivot for newly created Sprites.
		/// </summary>
		Vector2 pivot = Vector2(0.5f, 0.5f);
	};
}