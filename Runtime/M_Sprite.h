#pragma once
#include "M_Asset.h"

#include "M_CoordinateMode.h"
#include "M_Vector.h"

namespace minty
{
	class Texture;
	class Material;

	/// <summary>
	/// Holds data to create a new Sprite.
	/// </summary>
	struct SpriteBuilder
	{
		UUID id;

		Path path;

		/// <summary>
		/// The Texture that is being used.
		/// </summary>
		Texture* texture = nullptr;

		/// <summary>
		/// The Material to use.
		/// </summary>
		Material* material = nullptr;

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

	/// <summary>
	/// Holds the data for a slice of a texture.
	/// </summary>
	class Sprite
		: public Asset
	{
	private:
		Texture* _texture;
		Material* _material;

		Vector2 _minCoords;
		Vector2 _maxCoords;
		Vector2 _pivot;
	public:
		/// <summary>
		/// Creates an empty Sprite.
		/// </summary>
		Sprite();

		Sprite(SpriteBuilder const& builder, Runtime& engine);

		~Sprite();

		/// <summary>
		/// Destroys all of the resources associated with this Sprite.
		/// </summary>
		void destroy();

#pragma region Get

	public:
		Texture* get_texture() const;

		Material* get_material() const;

		Vector2 get_min_coords() const;

		Vector2 get_max_coords() const;

		Vector2 get_pivot() const;

#pragma endregion

#pragma region Set

	public:
		void set_min_coords(Vector2 const coords, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		void set_max_coords(Vector2 const coords, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

		void set_pivot(Vector2 const pivot, CoordinateMode const coordinateMode = CoordinateMode::Normalized);

	private:
		Vector2 normalize_coords(Vector2 const coords) const;

#pragma endregion
	};
}