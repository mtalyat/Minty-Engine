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
	class EditorApplication
		: public minty::Application
	{
	private:
		constexpr static char const* NAME = "Minty Editor";
	private:
		// info needed for a loaded project:
		Project* _project;
		minty::UUID _sceneId;

		// editor windows to be drawn
		std::unordered_map<minty::String, EditorWindow*> _editorWindows;
	public:
		EditorApplication();

		~EditorApplication();

	protected:
		void init(minty::RuntimeBuilder* builder = nullptr) override;

		/// <summary>
		/// Runs the game engine application.
		/// </summary>
		/// <param name="argc">The command line argument count.</param>
		/// <param name="argv">The command line arguments.</param>
		void loop() override;

		void destroy() override;

	public:
		void draw();

		void refresh();

	protected:
		minty::Runtime* create_runtime() override;

		minty::Window* create_window() override;

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

		void close_project();

		void load_project(minty::Path const& path);

		void unload_project();

		void create_new_project(minty::String const& name, minty::Path const& path);

#pragma endregion

#pragma region Scene

	private:
		void open_scene();

		void save_scene();

		void close_scene();

		void load_scene(minty::Path const& path);

		void unload_scene();

#pragma endregion

#pragma region Assets

	public:
		void copy_asset(minty::UUID const id) const;

		bool is_asset_copied(minty::String const& name) const;

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
		void clean_project();

		/// <summary>
		/// Builds the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void build_project(BuildInfo const& buildInfo);

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void run_project(BuildInfo const& buildInfo);

#pragma region File Generation

	private:
		void generate_directory(minty::Path const& path) const;

		void generate_directories(minty::Path const& basePath) const;

		void generate_application_data(BuildInfo const& buildInfo);

		void generate_wraps(BuildInfo const& buildInfo);

		void generate_assembly(BuildInfo const& buildInfo);

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

#pragma endregion

#pragma endregion
	};

	template<typename T>
	T* EditorApplication::find_editor_window(minty::String const& name)
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