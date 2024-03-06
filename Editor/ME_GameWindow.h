#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class GameWindow
		: public EditorWindow
	{
	private:

	public:
		GameWindow(EditorApplication& application);

		void draw() override;

		void reset() override;
	};
}