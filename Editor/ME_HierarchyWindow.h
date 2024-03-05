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
		minty::Entity _clicked;

	public:
		HierarchyWindow(Application& application);

		void draw() override;

		void reset() override;

		void set_project(Project* const project) override;

		void set_scene(minty::Scene* const scene) override;

	private:
		void set_clicked(minty::Entity const entity);

		void set_selected(minty::Entity const entity);

		void draw_popup(minty::EntityRegistry& entityRegistry);
	};
}