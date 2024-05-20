#pragma once

#include "types/M_Matrix.h"
#include "types/M_Vector.h"

namespace minty
{
	struct DrawCallObject3D
	{
		alignas(16) Matrix4 transform;
	};
}