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

		virtual void draw() override;

		virtual void reset() override;
	};
}