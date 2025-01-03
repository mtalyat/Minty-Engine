#pragma once
#include "EditorWindow.h"

namespace Mintye
{
	class HierarchyWindow
		: public EditorWindow
	{
	private:
		std::vector<Minty::String> m_registeredSystems;
		Minty::Entity m_selected;
		Minty::Entity m_clicked;

	public:
		HierarchyWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

		void set_project(Project* const project) override;

		void set_scene(Minty::Ref<Minty::Scene> const scene) override;

	private:
		Minty::Entity create_entity();

		void copy_entity(Minty::Entity const entity);

		Minty::Entity paste_entity();

		void destroy_entity(Minty::Entity const entity);

		Minty::Entity clone_entity(Minty::Entity const entity);

		void focus_entity(Minty::Entity const entity);

		void set_clicked(Minty::Entity const entity);

		void set_selected(Minty::Entity const entity);

		void draw_popup();

		void run_shortcuts();
	};
}