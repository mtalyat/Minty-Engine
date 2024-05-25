#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Types/M_Matrix.h"
#include "Minty/Types/M_Color.h"

namespace Minty
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
		Color _color;

	public:
		Camera(Perspective const perspective = Perspective::Perspective, float const fov = 45.0f, float const nearPlane = 0.1f, float const farPlane = 1000.0f);

#pragma region Get

	public:
		Perspective get_perspective() const { return _perspective; }

		float get_fov() const { return _fov * Math::RAD2DEG; }

		float get_near() const { return _near; }

		float get_far() const { return _far; }

		Color get_color() const { return _color; }

#pragma endregion

#pragma region Set

	public:
		void set_perspective(Perspective const perspective) { _perspective = perspective; }

		void set_fov(float const fov) { _fov = fov * Math::DEG2RAD; }

		void set_near(float const nearPlane) { _near = nearPlane; }

		void set_far(float const farPlane) { _far = farPlane; }

		void set_color(Color const color) { _color = color; }

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}