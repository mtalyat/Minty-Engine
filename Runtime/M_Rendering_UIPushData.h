#pragma once

#include "M_Base.h"
#include "M_Vector.h"
#include "M_Matrix.h"

namespace minty::rendering
{
	struct UIPushData
	{
		alignas(16) Vector2 minCoords;
		Vector2 maxCoords;
		alignas(16) Vector2 position;
		Vector2 size;
		alignas(16) int anchor;
		int unused0, unused1, unused2;
	};
}