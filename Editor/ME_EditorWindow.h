#pragma once

#include "ME_Minty.h"

namespace mintye
{
	class Application;
	class Project;

	class EditorWindow
		: public minty::Object
	{
	private:
		Application* _application;
		Project* _project;
		minty::Engine* _engine;
		minty::Scene* _scene;

	public:
		EditorWindow(Application& application);

		virtual ~EditorWindow();

	protected:
		Application& get_application() const;

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
	};
}