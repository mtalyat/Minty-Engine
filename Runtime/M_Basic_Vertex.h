#pragma once

#include "glm.hpp"

namespace minty::basic
{
	struct Vertex
	{
		glm::vec3 pos; // position of vertex
		glm::vec3 normal; // normal of vertex
		glm::vec2 coord; // fragment coordinate of vertex
	};
}