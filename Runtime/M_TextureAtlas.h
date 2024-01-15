#pragma once

#include "M_Texture.h"
#include "M_Rendering_TextureAtlasBuilder.h"
#include "M_CoordinateMode.h"
#include "M_Vector.h"

namespace minty
{
	/// <summary>
	/// A collection of Sprites that originate from a Texture.
	/// </summary>
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
		void set_slice(Vector2 const size, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		void set_pivot(Vector2 const size, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		Vector2Int get_size_in_slices() const;

		Vector2Int get_size() const;

		ID slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized) const;
		
		ID slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized) const;

		/// <summary>
		/// Creates a new Sprite using the given X and Y slice coordinates.
		/// </summary>
		/// <param name="x">The X slice coordinate.</param>
		/// <param name="y">The Y slice coordiante.</param>
		/// <param name="pivot">The pivot of the newly created Sprite.</param>
		/// <param name="coordinateMode">The coordinate mode to interpret the pivot and size in.</param>
		/// <returns>The ID to the newly created Sprite.</returns>
		ID create_sprite(int const x, int const y, Vector2 const pivot = Vector2(0.5f, 0.5f), CoordinateMode const coordinateMode = CoordinateMode::Normalized) const;

		/// <summary>
		/// Creates a new Sprite using the given X and Y slice coordinates.
		/// </summary>
		/// <param name="x">The X slice coordinate.</param>
		/// <param name="y">The Y slice coordiante.</param>
		/// <param name="pivot">The pivot of the newly created Sprite.</param>
		/// <param name="name">The name of the new Sprite.</param>
		/// <param name="coordinateMode">The coordinate mode to interpret the pivot and size in.</param>
		/// <returns>The ID to the newly created Sprite.</returns>
		ID create_sprite(int const x, int const y, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized) const;
	};
}