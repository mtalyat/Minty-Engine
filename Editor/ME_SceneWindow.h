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

		virtual void draw() override;

		virtual void reset() override;
	};
}