#pragma once

#include "M_Base.h"
#include "M_Vector.h"
#include "M_Matrix.h"

namespace minty
{
	struct SpritePushData
	{
		alignas(16) Matrix4 transform;
		Vector4 color;
		Vector2 minCoords;
		Vector2 maxCoords;
		Vector2 pivot;
		Vector2 size;
	};
}