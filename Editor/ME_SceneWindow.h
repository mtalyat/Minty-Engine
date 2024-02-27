#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class SceneWindow
		: public EditorWindow
	{
	private:

	public:
		SceneWindow(Application& application);

		void draw() override;

		void reset() override;
	};
}