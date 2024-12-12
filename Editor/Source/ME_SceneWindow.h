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
		void focus(Minty::Entity const entity);
	};
}