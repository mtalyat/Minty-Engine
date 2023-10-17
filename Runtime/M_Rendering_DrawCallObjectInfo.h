#pragma once

#include "M_Matrix.h"

namespace minty::rendering
{
	struct DrawCallObjectInfo
	{
		alignas(16) Matrix4 transform;
		alignas(16) minty::ID materialId;
	};

	struct CameraBufferObject {
		alignas (16) Matrix4 transform;
	};
}