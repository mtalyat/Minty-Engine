#pragma once

#include "M_Object.h"
#include "M_Vector.h"
#include "M_Matrix.h"
#include "M_Quaternion.h"

namespace minty
{
	/// <summary>
	/// Holds the positional, rotational and scale values for an object within the Scene.
	/// </summary>
	struct Transform
		: public Object
	{
		/// <summary>
		/// The position.
		/// </summary>
		Vector3 position;

		/// <summary>
		/// The rotation.
		/// </summary>
		Quaternion rotation;

		/// <summary>
		/// The scale.
		/// </summary>
		Vector3 scale;

		/// <summary>
		/// Creates a new Transform.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="rotation"></param>
		/// <param name="scale"></param>
		Transform(Vector3 const& position = Vector3(), Vector3 const& rotation = Vector3(), Vector3 const scale = Vector3(1.0f, 1.0f, 1.0f));

		/// <summary>
		/// Gets the Matrix containing the position, rotation and scale.
		/// </summary>
		/// <returns></returns>
		Matrix4 get_matrix() const;

	public:
		friend String to_string(Transform const& value);
	};
}