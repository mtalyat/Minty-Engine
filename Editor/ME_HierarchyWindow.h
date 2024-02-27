#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class HierarchyWindow
		: public EditorWindow
	{
	private:
		std::vector<minty::String> _registeredSystems;

	public:
		HierarchyWindow(Application& application);

		void draw() override;

		void reset() override;

		void set_project(Project* const project) override;
	};
}