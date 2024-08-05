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

	struct CameraBuilder
	{
		Perspective perspective = Perspective::Perspective;
		float fov = 45.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		Color color = Color::black();
		float aspectRatio = 16.0f / 9.0f;
		float size = 5.0f;
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
		float _aspectRatio;
		float _size;

	public:
		Camera(CameraBuilder const& builder = {});

#pragma region Get

	public:
		Perspective get_perspective() const { return _perspective; }

		float get_fov() const { return _fov * Math::RAD2DEG; }

		float get_near() const { return _near; }

		float get_far() const { return _far; }

		Color get_color() const { return _color; }

		float get_aspect_ratio() const { return _aspectRatio; }

		float get_size() const { return _size; }

#pragma endregion

#pragma region Set

	public:
		void set_perspective(Perspective const perspective) { _perspective = perspective; }

		void set_fov(float const fov) { _fov = fov * Math::DEG2RAD; }

		void set_near(float const nearPlane) { _near = nearPlane; }

		void set_far(float const farPlane) { _far = farPlane; }

		void set_color(Color const color) { _color = color; }

		void set_aspect_ratio(float const aspectRatio) { _aspectRatio = aspectRatio; }

		void set_size(float const size) { _size = size; }

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}