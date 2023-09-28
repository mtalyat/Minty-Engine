#pragma once

#include "M_Object.h"
#include "M_Vector3.h"

namespace minty
{
	struct Transform
		: public Object
	{
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};
}