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

		virtual void draw() override;

		virtual void reset() override;
	};
}