#pragma once

#include "glm.hpp"
#include "M_Vector.h"

namespace minty
{
	//typedef glm::quat Quaternion;

	struct Quaternion
		: public glm::quat
	{
		using glm::quat::qua;

		Quaternion(glm::quat const& other);

		Vector3 forward() const;

		Vector3 backward() const;

		Vector3 right() const;

		Vector3 left() const;

		Vector3 up() const;

		Vector3 down() const;
	};

	std::string to_string(Quaternion const& value);
}