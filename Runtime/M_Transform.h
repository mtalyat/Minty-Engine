#pragma once

#include "M_Object.h"
#include "M_Vector.h"
#include "M_Matrix.h"
#include "M_Quaternion.h"

namespace minty
{
	struct Transform
		: public Object
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		Transform(Vector3 const& position = Vector3(), Vector3 const& rotation = Vector3(), Vector3 const scale = Vector3(1.0f, 1.0f, 1.0f));

		Matrix4 get_matrix() const;
	};
}