#pragma once

#include "ME_Minty.h"

namespace mintye
{
	class EditorApplication;
	class Project;

	class EditorWindow
		: public minty::Object
	{
	private:
		EditorApplication* _application;
		Project* _project;
		minty::Engine* _engine;
		minty::Scene* _scene;

	public:
		EditorWindow(EditorApplication& application);

		virtual ~EditorWindow();

	protected:
		EditorApplication& get_application() const;

		minty::Runtime& get_runtime() const;

		Project* get_project() const;

		minty::Scene* get_scene() const;

	public:
		virtual void set_project(Project* const project);

		virtual void set_scene(minty::Scene* const scene);

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