#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class HierarchyWindow
		: public EditorWindow
	{
	private:
		std::vector<minty::String> _registeredSystems;
		minty::Entity _selected;

	public:
		HierarchyWindow(Application& application);

		void draw() override;

		void reset() override;

		void set_project(Project* const project) override;

		void set_scene(minty::Scene* const scene) override;
	};
}