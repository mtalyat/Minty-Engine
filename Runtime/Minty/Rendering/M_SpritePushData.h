#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_Matrix.h"

namespace Minty
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