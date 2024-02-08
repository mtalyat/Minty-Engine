#pragma once

#include "M_Matrix.h"
#include "M_Vector.h"

namespace minty::Builtin
{
	// global uniform buffer object
	struct GlobalUBO
	{
		alignas(16) float time;
		float deltaTime;
		float unused0;
		float unused1;

		alignas(16) Matrix4 view;
	};

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