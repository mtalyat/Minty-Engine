#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Rendering/M_CoordinateMode.h"
#include "Minty/Types/M_Vector.h"

namespace Minty
{
	class RenderSystem;
	class Texture;
	class Material;
	class Sprite;

	/// <summary>
	/// Holds data to create a new TextureAtlas.
	/// </summary>
	struct TextureAtlasBuilder
	{
		UUID id;

		Path path;

		/// <summary>
		/// The ID of the Texture to be used.
		/// </summary>
		Texture* texture = nullptr;

		/// <summary>
		/// The ID of the Material to be used. If left as ERROR_ID, a new Material will be generated using the texture.
		/// </summary>
		Material* material = nullptr;

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

	/// <summary>
	/// A collection of Sprites that originate from a Texture.
	/// </summary>
	class TextureAtlas
		: public Asset
	{
	private:
		Ref<Texture> _texture;
		Ref<Material> _material;

		// size of slice in atlas, in pixels
		Vector2 _slice;
		// pivot of each sprite when sliced, in pixels
		Vector2 _pivot;
		// number of slices that have been made
		size_t _createdSlices;
	public:
		/// <summary>
		/// Creates an empty TextureAtlas.
		/// </summary>
		TextureAtlas() = default;

		TextureAtlas(TextureAtlasBuilder const& builder);

		~TextureAtlas() = default;

		/// <summary>
		/// Sets the size of the sliced cells within this TextureAtlas.
		/// </summary>
		/// <param name="size">The size of the cell.</param>
		/// <param name="coordinateMode"></param>
		void set_slice(Vector2 const size, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		void set_pivot(Vector2 const size, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		Vector2Int get_size_in_slices() const;

		Vector2Int get_size() const;

		Ref<Sprite> slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		Ref<Sprite> slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized);
		
		Ref<Sprite> slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		/// <summary>
		/// Creates a new Sprite using the given X and Y slice coordinates.
		/// </summary>
		/// <param name="x">The X slice coordinate.</param>
		/// <param name="y">The Y slice coordiante.</param>
		/// <param name="pivot">The pivot of the newly created Sprite.</param>
		/// <param name="coordinateMode">The coordinate mode to interpret the pivot in.</param>
		/// <returns>The ID to the newly created Sprite.</returns>
		Ref<Sprite> create_sprite(int const x, int const y, Vector2 const pivot = Vector2(0.5f, 0.5f), CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		/// <summary>
		/// Creates a new Sprite using the given X and Y slice coordinates.
		/// </summary>
		/// <param name="x">The X slice coordinate.</param>
		/// <param name="y">The Y slice coordiante.</param>
		/// <param name="pivot">The pivot of the newly created Sprite.</param>
		/// <param name="name">The name of the new Sprite.</param>
		/// <param name="coordinateMode">The coordinate mode to interpret the pivot in.</param>
		/// <returns>The ID to the newly created Sprite.</returns>
		Ref<Sprite> create_sprite(int const x, int const y, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		/// <summary>
		/// Creates a Sprite of all possible full slices that can be created within this TextureAtlas.
		/// </summary>
		/// <param name="pivot">The pivot to apply to all the Sprites.</param>
		/// <param name="coordinateMode">The coordinate mode to interpret the pivot in.</param>
		/// <returns>A list of all new Sprite IDs.</returns>
		std::vector<Ref<Sprite>> create_all(Vector2 const pivot = Vector2(0.5f, 0.5f), CoordinateMode const coordinateMode = CoordinateMode::Normalized);
	private:
		String get_next_slice_name() const;
	};
}