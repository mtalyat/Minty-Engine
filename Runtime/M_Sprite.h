#pragma once

#include "M_Object.h"
#include "M_Vector.h"

namespace minty
{
	class Sprite
		: public Object
	{
	private:
		ID _materialId;

		Vector2 _min;
		Vector2 _max;

	public:
		Sprite(ID const materialId);

		Sprite(ID const materialId, Vector2 minCoords, Vector2 maxCoords);

		ID get_material_id() const;

		Vector2 get_min_coords() const;

		Vector2 get_max_coords() const;

		void set_min_coords(Vector2 const coords);

		void set_max_coords(Vector2 const coords);
	};
}