#pragma once

#include "M_Types.h"
#include "M_GLM.hpp"

namespace minty
{
	typedef glm::mat2 Matrix2;
	typedef glm::mat3 Matrix3;
	typedef glm::mat4 Matrix4;

	String to_string(Matrix2 const& value);
	String to_string(Matrix3 const& value);
	String to_string(Matrix4 const& value);
}