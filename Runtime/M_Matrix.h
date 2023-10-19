#pragma once

#include "M_Base.h"
#include "glm.hpp"

namespace minty
{
	typedef glm::mat4 Matrix4;

	std::string to_string(Matrix4 const& value);
}