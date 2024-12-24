#pragma once
#include "ME_Minty.h"

#include "ME_BuildInfo.h"
#include "ME_FileWatcher.h"
#include <unordered_map>
#include <vector>

namespace Mintye
{
	class Project;
	class EditorWindow;

	class EditorApplicationData
		: public Minty::Serializable
	{
	private:
		static constexpr int MAX_RECENT_PROJECTS = 10;
		std::vector<Minty::Path> _recentProjects;

	public:
		EditorApplicationData()
			: _recentProjects()
		{}

		~EditorApplicationData() = default;

	public:
		void emplace_recent_project(Minty::Path const& path);

		void erase_recent_project(Minty::Path const& path);

		std::vector<Minty::Path> const& get_recent_projects() const;

	public:
		void serialize(Minty::Writer& writer) const override;
		void deserialize(Minty::Reader& reader) override;
	};

	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class EditorApplication
		: public Minty::Application
	{
	private:
		constexpr static char const* NAME = "Minty Editor";
	private:
		EditorApplicationData m_data;
		BuildInfo m_buildInfo;

		// info needed for a loaded project:
		Minty::Path m_projectPath;
		Project* mp_project;
		FileWatcher* mp_watcher;
		Minty::UUID m_sceneId;

		// starting working directory
		Minty::Path m_cwd;

		// editor windows to be drawn
		std::unordered_map<Minty::String, EditorWindow*> m_editorWindows;

		// tasks being ran (such as building)
		Minty::TaskFactory<void> m_taskFactory;
	public:
		EditorApplication();

		~EditorApplication();

	public:
		void refresh();

	protected:
		Minty::Int update(Minty::Time const& time) override;
		Minty::Int update_gui(Minty::Time const& time) override;

	private:
		void cwd_application() const;

		void cwd_project() const;

#pragma region Data

	private:
		void load_data();

		void save_data() const;

#pragma endregion

#pragma region Set

	private:
		void set_project(Project* const project);

		void set_scene(Minty::UUID const sceneId);

		void set_window_title(Minty::String const& subTitle);

#pragma endregion

#pragma region Drawing

	private:
		void draw_dock_space();

		Minty::Int draw_menu_bar();

		void draw_commands();

		void draw_editor_windows();

		void reset_editor_windows();

	public:
		template<typename T>
		T* find_editor_window(Minty::String const& name);

#pragma endregion

#pragma region Project

	private:
		void new_project();

		void open_project();

		void open_project_directory() const;

		void save_project();

		void close_project();

		void load_most_recent_project();

		void load_project(Minty::Path const& path);

		void unload_project();

		void create_new_project(Minty::String const& name, Minty::Path const& path);

		Minty::Path get_project_dll_path() const;

		void reload_project();

#pragma endregion

#pragma region Scene

	private:
		void open_scene();

		void save_scene();

		void close_scene();

		void load_scene(Minty::Path const& path);

		void unload_scene();

#pragma endregion

#pragma region Assets

	public:
		void copy_asset(Minty::UUID const id) const;

		bool is_asset_copied(Minty::String const& name) const;

		/// <summary>
		/// Opens the asset in the editor, or in the system if the editor does not support it
		/// </summary>
		/// <param name="path"></param>
		void open_asset(Minty::Path const& path);

		/// <summary>
		/// Gets the name of the thing with the given ID. Could be an Entity or Asset.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Minty::String get_name(Minty::UUID const id) const;

		void create_asset(Minty::Path const& path);

		void create_directory(Minty::Path const& path);

	private:
		void create_asset(Minty::Path const& path, std::unordered_map<Minty::String, Minty::String> const& params);

		Minty::Path find_template(Minty::Path const& extension);

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
		void build_project();

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void run_project();

#pragma region Files

	private:
		void generate_directory(Minty::Path const& path) const;

		void generate_directories(Minty::Path const& basePath) const;

		void generate_application_data();

		void generate_wraps();

		void generate_assembly();

		/// <summary>
		/// Generates and updates the cmake file for the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void generate_cmake();

		/// <summary>
		/// Generates the main file for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate_main();

		/// <summary>
		/// Copies all of the necessary files to run an exe.
		/// </summary>
		void copy_files();

#pragma endregion

#pragma endregion

#pragma region Logging

	public:
		void log(Minty::String const& message);

		void log_warning(Minty::String const& message);

		void log_error(Minty::String const& message);

		void log_info(Minty::String const& message);

#pragma endregion

#pragma region Shortcuts

	private:
		void run_shortcuts();

#pragma endregion

#pragma region Theme

	private:
		void load_theme();

#pragma endregion

	};

	template<typename T>
	T* EditorApplication::find_editor_window(Minty::String const& name)
	{
		auto found = m_editorWindows.find(name);

		if (found == m_editorWindows.end())
		{
			// none with that name
			MINTY_ERROR_FORMAT("No editor window with the message \"{}\" exists.", name);
			return nullptr;
		}

		return static_cast<T*>(found->second);
	}
}