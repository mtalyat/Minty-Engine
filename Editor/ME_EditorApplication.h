#pragma once
#include "ME_Minty.h"

#include "ME_BuildInfo.h"
#include "ME_FileWatcher.h"
#include <unordered_map>
#include <vector>

namespace mintye
{
	class Project;
	class EditorWindow;

	class EditorApplicationData
		: public minty::Object
	{
	private:
		static constexpr int MAX_RECENT_PROJECTS = 10;
		std::vector<minty::Path> _recentProjects;

	public:
		EditorApplicationData();

	public:
		void emplace_recent_project(minty::Path const& path);

		void erase_recent_project(minty::Path const& path);

		std::vector<minty::Path> const& get_recent_projects() const;

	public:
		void serialize(minty::Writer& writer) const override;
		void deserialize(minty::Reader const& reader) override;
	};

	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class EditorApplication
		: public minty::Application
	{
	private:
		constexpr static char const* NAME = "Minty Editor";
	private:
		EditorApplicationData _data;
		BuildInfo _buildInfo;

		// info needed for a loaded project:
		Project* _project;
		FileWatcher* _watcher;
		minty::UUID _sceneId;

		// starting working directory
		minty::Path _cwd;

		// editor windows to be drawn
		std::unordered_map<minty::String, EditorWindow*> _editorWindows;
	public:
		EditorApplication();

		~EditorApplication();

	protected:
		void init(minty::RuntimeBuilder* builder = nullptr) override;

		void destroy() override;

	public:
		void draw();

		void refresh();

	protected:
		minty::Runtime* create_runtime() override;

		minty::Window* create_window() override;

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

		void set_scene(minty::UUID const sceneId);

		void set_window_title(minty::String const& subTitle);

#pragma endregion

#pragma region Drawing

	private:
		void draw_dock_space();

		void draw_menu_bar();

		void draw_commands();

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

		void load_most_recent_project();

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

		/// <summary>
		/// Gets the name of the thing with the given ID. Could be an Entity or Asset.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		minty::String get_name(minty::UUID const id) const;

		void create_asset(minty::Path const& path);

		void create_directory(minty::Path const& path);

	private:
		void create_asset(minty::Path const& path, std::unordered_map<minty::String, minty::String> const& params);

		minty::Path find_template(minty::Path const& extension);

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

#pragma region File Generation

	private:
		void generate_directory(minty::Path const& path) const;

		void generate_directories(minty::Path const& basePath) const;

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

#pragma endregion

#pragma endregion

#pragma region Logging

	public:
		void log(minty::String const& message);

		void log_warning(minty::String const& message);

		void log_error(minty::String const& message);

#pragma endregion

	};

	template<typename T>
	T* EditorApplication::find_editor_window(minty::String const& name)
	{
		auto found = _editorWindows.find(name);

		if (found == _editorWindows.end())
		{
			// none with that name
			MINTY_ERROR_FORMAT("No editor window with the message \"{}\" exists.", name);
			return nullptr;
		}

		return static_cast<T*>(found->second);
	}
}