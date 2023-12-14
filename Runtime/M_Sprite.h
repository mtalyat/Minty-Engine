#pragma once

#include "M_Object.h"
#include "M_Vector.h"

namespace minty
{
	/// <summary>
	/// Holds the data for a slice of a texture.
	/// </summary>
	class Sprite
		: public Object
	{
	private:
		ID _materialId;

		Vector2 _min;
		Vector2 _max;
		Vector2 _pivot;

	public:
		Sprite(ID const materialId);

		Sprite(ID const materialId, Vector2 const pivot);

		Sprite(ID const materialId, Vector2 const minCoords, Vector2 const maxCoords);

		Sprite(ID const materialId, Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot);

#pragma region Get

		ID get_material_id() const;

		Vector2 get_min_coords() const;

		Vector2 get_max_coords() const;

		Vector2 get_pivot() const;

#pragma endregion

#pragma region Set

		void set_min_coords(Vector2 const coords);

		void set_max_coords(Vector2 const coords);

		void set_pivot(Vector2 const pivot);

#pragma endregion
	};
}