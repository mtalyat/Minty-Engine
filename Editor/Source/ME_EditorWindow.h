#pragma once

#include "ME_Minty.h"

namespace Mintye
{
	class EditorApplication;
	class Project;

	class EditorWindow
	{
	private:
		EditorApplication* _application;
		Project* _project;
		Minty::Ref<Minty::Scene> _scene;

	public:
		EditorWindow(EditorApplication& application);

		virtual ~EditorWindow();

	protected:
		EditorApplication& get_application() const;

		Project* get_project() const;

		Minty::Ref<Minty::Scene> get_scene() const;

	public:
		virtual void set_project(Project* const project);

		virtual void set_scene(Minty::Ref<Minty::Scene> const scene);

		/// <summary>
		/// Draws the window using ImGui.
		/// </summary>
		virtual void draw() = 0;

		/// <summary>
		/// Clears all existing data and sets back to defaults.
		/// </summary>
		virtual void reset() = 0;

		/// <summary>
		/// Refreshes the data loaded in memory when something changes on the disc.
		/// </summary>
		virtual void refresh() = 0;
	};
}