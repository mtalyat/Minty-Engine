#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_SpriteBuilder.h"
#include "M_Vector.h"

namespace minty
{
	/// <summary>
	/// Holds the data for a slice of a texture.
	/// </summary>
	class Sprite
		: public rendering::RenderObject
	{
	private:
		ID _textureId;
		ID _materialId;

		Vector2 _minCoords;
		Vector2 _maxCoords;
		Vector2 _pivot;
	public:
#pragma region Get
		Sprite(rendering::SpriteBuilder const& builder, RenderEngine& renderer);

		/// <summary>
		/// Destroys all of the resources associated with this Sprite.
		/// </summary>
		void destroy();

		ID get_material_id() const;

		Vector2 get_min_coords() const;

		Vector2 get_max_coords() const;

		Vector2 get_pivot() const;
#pragma endregion

#pragma region Set

	public:
		void set_min_coords(Vector2 const coords, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized);

		void set_max_coords(Vector2 const coords, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized);

		void set_pivot(Vector2 const pivot, PixelCoordinateMode const coordinateMode = PixelCoordinateMode::Normalized);

	private:
		Vector2 normalize_coords(Vector2 const coords) const;

#pragma endregion
	};
}