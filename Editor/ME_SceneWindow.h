#pragma once
#include "ME_EditorWindow.h"

#include "ME_Minty.h"

namespace Mintye
{
	class SceneWindow
		: public EditorWindow
	{
	private:
		Minty::Vector3 _cameraPosition;
		Minty::Vector3 _cameraOrientation;
		Minty::Quaternion _cameraRotation;
		Minty::Camera _camera;

		bool _isMouseMoving;
		bool _isMouseRotating;
		ImVec2 _mouseDragPos;

	public:
		SceneWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

	public:
		void focus(Minty::Entity const entity);
	};
}