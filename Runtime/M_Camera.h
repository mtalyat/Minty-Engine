#pragma once
#include "M_Object.h"

#include "M_Matrix.h"

namespace minty
{
	enum class Perspective : int
	{
		/// <summary>
		/// Perspective camera view.
		/// </summary>
		Perspective,

		/// <summary>
		/// Orthographic camera view.
		/// </summary>
		Orthographic,
	};

	String to_string(Perspective const value);
	Perspective from_string_perspective(String const& value);

	struct CameraBufferObject {
		alignas (16) Matrix4 transform;
	};

	class Camera
		: public Object
	{
	private:
		Perspective _perspective;
		float _fov;
		float _near;
		float _far;

	public:
		Camera(Perspective const perspective = Perspective::Perspective, float const fov = 45.0f, float const nearPlane = 0.1f, float const farPlane = 1000.0f);

#pragma region Get

	public:
		Perspective get_perspective() const;

		float get_fov() const;

		float get_near() const;

		float get_far() const;

#pragma endregion

#pragma region Set

	public:
		void set_perspective(Perspective const perspective);

		void set_fov(float const fov);

		void set_near(float const nearPlane);

		void set_far(float const farPlane);

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}