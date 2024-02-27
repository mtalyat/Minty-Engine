#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class HierarchyWindow
		: public EditorWindow
	{
	private:

	public:
		HierarchyWindow(Application& application);

		void draw() override;

		void reset() override;
	};
}