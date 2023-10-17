#pragma once

#include "glm.hpp"

namespace minty::basic
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 coord;
	};
}