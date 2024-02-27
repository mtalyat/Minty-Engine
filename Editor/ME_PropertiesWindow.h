#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class PropertiesWindow
		: public EditorWindow
	{
	private:

	public:
		PropertiesWindow(Application& application);

		void draw() override;

		void reset() override;
	};
}