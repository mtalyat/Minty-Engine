#pragma once

#include "glm.hpp"
#include "M_Vector.h"

namespace minty::basic
{
	struct Vertex3D
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 coord;
	};

	struct Vertex2D
	{
		Vector2 pos;
		Vector2 coord;
	};
}