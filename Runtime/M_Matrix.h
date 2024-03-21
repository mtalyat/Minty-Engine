#pragma once

#include "M_Types.h"
#include "M_Vector.h"
#include "M_Quaternion.h"
#include "M_GLM.hpp"

namespace minty
{
	typedef glm::mat2 Matrix2;
	typedef glm::mat3 Matrix3;
	typedef glm::mat4 Matrix4;

	Vector3 matrix4_get_position(Matrix4 const& value);
	Quaternion matrix4_get_rotation(Matrix4 const& value);
	Quaternion matrix4_get_rotation(Matrix4 const& value, Vector3 const scale);
	Vector3 matrix4_get_scale(Matrix4 const& value);
	void matrix4_set_position(Matrix4& value, Vector3 const position);
	void matrix4_set_rotation(Matrix4& value, Quaternion const position);
	void matrix4_set_scale(Matrix4& value, Vector3 const position);

	String to_string(Matrix2 const& value);
	String to_string(Matrix3 const& value);
	String to_string(Matrix4 const& value);
}