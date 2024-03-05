#pragma once
#include "ME_EditorWindow.h"

#include "ME_Minty.h"

namespace mintye
{
	class SceneWindow
		: public EditorWindow
	{
	private:
		minty::Vector3 _cameraPosition;
		minty::Vector3 _cameraOrientation;
		minty::Quaternion _cameraRotation;
		minty::Camera _camera;

		bool _isMouseMoving;
		bool _isMouseRotating;
		ImVec2 _mouseDragPos;

	public:
		SceneWindow(Application& application);

		void draw() override;

		void reset() override;
	};
}