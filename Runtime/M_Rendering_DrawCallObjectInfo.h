#pragma once

#include "glm.hpp"

namespace minty::rendering
{
	struct DrawCallObjectInfo
	{
		glm::mat4 transform;
	};

	struct CameraBufferObject {
		alignas (16) glm::mat4 transform;
	};

	struct MaterialBufferObject
	{
		alignas (16) minty::ID textureID;
		alignas (16) glm::vec4 color;
	};
}