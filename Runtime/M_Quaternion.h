#pragma once

#include "M_Object.h"
#include "M_Vector.h"
#include "M_GLM.hpp"

namespace minty
{
	/// <summary>
	/// A Quaternion.
	/// </summary>
	struct Quaternion
		: public glm::quat, public Object
	{
		using glm::quat::qua;
		Quaternion(glm::quat const& other);

		/// <summary>
		/// Gets the forward vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 forward() const;

		/// <summary>
		/// Gets the backward vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 backward() const;

		/// <summary>
		/// Gets the right vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 right() const;

		/// <summary>
		/// Gets the left vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 left() const;

		/// <summary>
		/// Gets the up vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 up() const;

		/// <summary>
		/// Gets the down vector from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 down() const;

		/// <summary>
		/// Gets the euler angles (x, y, z) from this Quaternion.
		/// </summary>
		/// <returns></returns>
		Vector3 to_euler_angles() const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Quaternion& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Quaternion& vector);

		/// <summary>
		/// Creates a Quaternion from the given euler angles.
		/// </summary>
		/// <param name="x">The rotation on the X axis.</param>
		/// <param name="y">The rotation on the Y axis.</param>
		/// <param name="z">The rotation on the Z axis.</param>
		/// <returns>A Quaternion representing the same orientation.</returns>
		static Quaternion from_euler_angles(float const x, float const y, float const z);

		/// <summary>
		/// Creates a Quaternion from the given euler angles.
		/// </summary>
		/// <param name="v">A Vector3 containing the X, Y and Z axes rotations.</param>
		/// <returns>A Quaternion representing the same orientation.</returns>
		static Quaternion from_euler_angles(Vector3 const v);

		friend String to_string(Quaternion const& value);
	};
}