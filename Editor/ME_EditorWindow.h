#pragma once

#include <Minty.h>

namespace mintye
{
	class Application;

	class EditorWindow
		: public minty::Object
	{
	private:
		Application* _application;

	public:
		EditorWindow(Application& application);

		virtual ~EditorWindow();

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