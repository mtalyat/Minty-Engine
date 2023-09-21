#pragma once

#include "M_Component.h"
#include "M_Vector3.h"

namespace minty
{
	struct TransformComponent
		: public Component
	{
		Vector3 position;
		Vector3 rotation; // TODO: replace with quaternion
		Vector3 scale;
	};
}