#pragma once
#include "ME_EditorWindow.h"

#include "ME_Minty.h"

namespace Mintye
{
	class SceneWindow
		: public EditorWindow
	{
	private:
		Minty::Float3 m_cameraPosition;
		Minty::Float3 m_cameraOrientation;
		Minty::Quaternion m_cameraRotation;
		Minty::Camera m_camera;

		bool m_isMouseMoving;
		bool m_isMouseRotating;
		Minty::Float2 m_mouseDragPos;

	public:
		SceneWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

	public:
		Minty::Float3 const& get_camera_position() const { return m_cameraPosition; }

		Minty::Float3 const& get_camera_oreintation() const { return m_cameraOrientation; }

		Minty::Camera const& get_camera() const { return m_camera; }

		void set_camera_position(Minty::Float3 const& position) { m_cameraPosition = position; }

		void set_camera_orientation(Minty::Float3 const& orientation) { m_cameraOrientation = orientation; }

		void set_camera(Minty::Camera const& camera) { m_camera = camera; }

	public:
		void focus(Minty::Entity const entity);
	};
}