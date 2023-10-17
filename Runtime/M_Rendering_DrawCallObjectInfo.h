#pragma once

#include "glm.hpp"

namespace minty::rendering
{
	struct DrawCallObjectInfo
	{
		alignas(16) glm::mat4 transform;
		alignas(16) minty::ID materialId;
	};

	struct CameraBufferObject {
		alignas (16) glm::mat4 transform;
	};
}