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
		HierarchyWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

		void set_project(Project* const project) override;

		void set_scene(minty::Scene* const scene) override;

	private:
		minty::Entity create_entity();

		void copy_entity(minty::Entity const entity);

		minty::Entity paste_entity();

		void destroy_entity(minty::Entity const entity);

		minty::Entity clone_entity(minty::Entity const entity);

		void focus_entity(minty::Entity const entity);

		void set_clicked(minty::Entity const entity);

		void set_selected(minty::Entity const entity);

		void draw_popup();

		void run_shortcuts();
	};
}