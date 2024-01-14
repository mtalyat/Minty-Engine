#pragma once

#include "M_Texture.h"
#include "M_Rendering_TextureAtlasBuilder.h"
#include "M_PixelCoordinateMode.h"
#include "M_Vector.h"

namespace minty
{
	class TextureAtlas
		: public rendering::RenderObject
	{
	private:
		// id to texture
		ID _textureId;
		// id to the material
		ID _materialId;
		// size of slice in atlas, in pixels
		Vector2 _slice;
		// pivot of each sprite when sliced, in pixels
		Vector2 _pivot;
	public:
		TextureAtlas(rendering::TextureAtlasBuilder const& builder, RenderEngine& renderer);

		~TextureAtlas();

		/// <summary>
		/// Sets the size of the sliced cells within this TextureAtlas.
		/// </summary>
		/// <param name="size">The size of the cell.</param>
		/// <param name="coordinateMode"></param>
		void set_slice(Vector2 const size, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized);

		void set_pivot(Vector2 const size, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized);

		Vector2Int get_size_in_slices() const;

		Vector2Int get_size() const;

		ID create_sprite(int const x, int const y, Vector2 const pivot = Vector2(0.5f, 0.5f), Vector2 const size = Vector2(1.0f, 1.0f), PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized) const;

		ID create_sprite(int const x, int const y, Vector2 const pivot, Vector2 const size, String const& name, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized) const;
	};
}