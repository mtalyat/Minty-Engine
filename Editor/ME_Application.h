#pragma once

#include "ME_Minty.h"
#include <unordered_map>

namespace mintye
{
	struct BuildInfo;
	class Project;
	class EditorWindow;

	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class Application
	{
	private:
		constexpr static char const* NAME = "Minty Editor";

	private:
		enum class NewProjectSetupType
		{
			/// <summary>
			/// Do nothing special.
			/// </summary>
			Default,

			/// <summary>
			/// Add .vscode folder, set up for debugging.
			/// </summary>
			VSCode,
		};

	private:
		// info needed for the editor:
		minty::Info _info;
		minty::Path _path;
		minty::Window _window;
		minty::Runtime* _runtime;

		// info needed for a loaded project:
		Project* _project;
		minty::UUID _sceneId;

		// editor windows to be drawn
		std::unordered_map<minty::String, EditorWindow*> _editorWindows;
	public:
		Application();

		~Application();

		/// <summary>
		/// Runs the game engine application.
		/// </summary>
		/// <param name="argc">The command line argument count.</param>
		/// <param name="argv">The command line arguments.</param>
		int run(int argc, char const* argv[]);

		minty::Runtime& get_runtime() const;
		
		void draw();

	private:
		void cleanup();

#pragma region Set

	private:
		void set_project(Project* const project);

		void set_scene(minty::UUID const sceneId);

		void set_window_title(minty::String const& subTitle);

#pragma endregion

#pragma region Drawing

	private:
		void draw_dock_space();

		void draw_menu_bar();

		void draw_commands(BuildInfo& buildInfo);

		void draw_editor_windows();

		void reset_editor_windows();

	public:
		template<typename T>
		T* find_editor_window(minty::String const& name);

#pragma endregion

#pragma region Project

	private:
		void new_project();

		void open_project();

		void save_project();

		void save_as_project();

		void close_project();

		void load_project(minty::Path const& path);

		void unload_project();

		void create_new_project(minty::String const& name, minty::Path const& path, NewProjectSetupType initType = NewProjectSetupType::Default);

#pragma endregion

#pragma region Scene

	private:
		void load_scene(minty::Path const& path);

		void unload_scene();

#pragma endregion

#pragma region Assets

	public:
		/// <summary>
		/// Opens the asset in the editor, or in the system if the editor does not support it
		/// </summary>
		/// <param name="path"></param>
		void open_asset(minty::Path const& path);

#pragma endregion

#pragma region Building and Running

	private:
		/// <summary>
		/// Cleans the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void clean();

		/// <summary>
		/// Builds the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void build(BuildInfo const& buildInfo);

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void run(BuildInfo const& buildInfo);

#pragma region File Generation

	private:
		/// <summary>
		/// Generates and updates the cmake file for the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void generate_cmake(BuildInfo const& buildInfo);

		/// <summary>
		/// Generates the main file for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate_main(BuildInfo const& buildInfo);

		/// <summary>
		/// Generates all of the files for a vscode project setup.
		/// </summary>
		void generate_vscode();

		/// <summary>
		/// Generates all of the init.h and init.cpp files for a project.
		/// </summary>
		void generate_init(minty::Path const& path);

#pragma endregion

#pragma endregion
	};

	template<typename T>
	T* Application::find_editor_window(minty::String const& name)
	{
		auto found = _editorWindows.find(name);

		if (found == _editorWindows.end())
		{
			// none with that name
			minty::Console::error(std::format("No editor window with the name \"{}\" exists.", name));
			return nullptr;
		}

		return static_cast<T*>(found->second);
	}
}