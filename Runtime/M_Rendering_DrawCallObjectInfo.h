#pragma once

#include "M_Matrix.h"
#include "M_Vector.h"

namespace minty::rendering
{
	struct DrawCallObject3D
	{
		alignas(16) Matrix4 transform;
		alignas(16) minty::ID materialId;
	};

	struct DrawCallObjectUI
	{
		minty::ID materialId;
		int x, y, z;
		minty::Vector4 coords;
		minty::Vector4 pos;
	};

	struct CameraBufferObject {
		alignas (16) Matrix4 transform;
	};
}