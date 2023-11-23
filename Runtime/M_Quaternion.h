#pragma once

#include "M_Base.h"
#include "M_Vector.h"
#include "glm.hpp"

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

		Vector3 to_euler_angles() const;

		static Quaternion from_euler_angles(float const x, float const y, float const z);

		static Quaternion from_euler_angles(Vector3 const v);

		friend std::string to_string(Quaternion const& value);
	};

	std::string to_string(glm::quat const& value);
}