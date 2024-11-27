#pragma once
#include "ME_EditorWindow.h"

namespace Mintye
{
	class GameWindow
		: public EditorWindow
	{
	private:

	public:
		GameWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;
	};
}