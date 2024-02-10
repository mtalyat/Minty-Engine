#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class GameWindow
		: public EditorWindow
	{
	private:

	public:
		GameWindow(Application& application);

		virtual void draw() override;

		virtual void reset() override;
	};
}