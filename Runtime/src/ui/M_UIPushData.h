#pragma once

#include "core/M_Base.h"
#include "types/M_Vector.h"
#include "types/M_Matrix.h"

namespace minty
{
	struct UIPushData
	{
		alignas(16) float x; // left
		float y; // top
		float width; // right
		float height; // bottom
		Vector4 color;
		alignas(16) int anchorMode;
		int unused0, unused1, unused2;
	};

	struct CanvasBufferObject
	{
		alignas(16) int width;
		int height;
		int unused0, unused1;
	};
}