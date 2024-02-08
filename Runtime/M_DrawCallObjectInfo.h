#pragma once

#include "M_Matrix.h"
#include "M_Vector.h"

namespace minty
{
	struct DrawCallObject3D
	{
		alignas(16) Matrix4 transform;
	};

	struct CameraBufferObject {
		alignas (16) Matrix4 transform;
	};
}