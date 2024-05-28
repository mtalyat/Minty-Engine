#pragma once

#include "Minty/Types/M_Matrix.h"
#include "Minty/Types/M_Vector.h"

namespace Minty
{
	struct DrawCallObject3D
	{
		alignas(16) Matrix4 transform;
	};
}