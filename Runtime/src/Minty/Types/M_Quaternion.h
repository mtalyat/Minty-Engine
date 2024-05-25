#pragma once

#include "Minty/Types/M_Object.h"
#include "Minty/Types/M_Vector.h"
#include "Minty/Libraries/M_GLM.hpp"

namespace Minty
{
	typedef glm::quat Quaternion;

	Vector3 forward(Quaternion const& value);
	Vector3 backward(Quaternion const& value);
	Vector3 right(Quaternion const& value);
	Vector3 left(Quaternion const& value);
	Vector3 up(Quaternion const& value);
	Vector3 down(Quaternion const& value);

	Vector3 to_euler(Quaternion const& value);
	Quaternion from_euler(Vector3 const& value);

	String to_string(Quaternion const& value);

	std::istream& operator>>(std::istream& stream, Quaternion& vector);
	std::ostream& operator<<(std::ostream& stream, const Quaternion& vector);
}