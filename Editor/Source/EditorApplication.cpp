#include "EditorApplication.h"

#include "Constants.h"
#include "Project.h"
#include "BuildInfo.h"

#include "ConsoleWindow.h"
#include "HierarchyWindow.h"
#include "SceneWindow.h"
#include "GameWindow.h"
#include "PropertiesWindow.h"
#include "AssetsWindow.h"
#include "Meta.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <filesystem> // for accessing file system
#include <fstream> // for accessing files
#include <chrono> // for time
#include <format>

#include <cstdio>
//#include <iostream>
#include <memory>
#include <stdexcept>
//#include <string>
#include <array>

#define CMAKE_PATH "cmake"

namespace fs = std::filesystem;
using namespace Mintye;
using namespace Minty;

static constexpr char const* DATA_FILENAME = "data.Minty";

EditorApplication::EditorApplication()
	: Application()
	, m_data()
	, m_buildInfo()
	, m_projectPath()
	, mp_project()
	, mp_projectData()
	, mp_watcher()
	, m_sceneId(INVALID_UUID)
	, m_cwd(std::filesystem::current_path())
	, m_editorWindows()
{
	// initialize self
	ApplicationBuilder builder{};
	builder.info.name = "Minty Editor";
	builder.info.version = MINTY_MAKE_VERSION(1, 0, 0);
	builder.logPath = "Log.txt";
	builder.mode = ApplicationMode::Edit;
	builder.targetFPS = 60;
	builder.passes = ApplicationPassFlags::All;
	builder.assetManagerBuilder.wraps = { "Default.wrap" };
	builder.assetManagerBuilder.savePaths = true;
	builder.rendererBuilder.clearColor = Color::black();
	builder.rendererBuilder.targetSurfaceFormat = Format::R8G8B8A8_SRGB;
	initialize(builder);

	// load gui theme
	load_theme();

	// load data
	load_data();

	// initialize meta
	MetaDatabase::initialize();

	// create all the editor windows
	// add to list so they get drawn and updated
	m_editorWindows.emplace("Console", new ConsoleWindow(*this));
	m_editorWindows.emplace("Hierarchy", new HierarchyWindow(*this));
	m_editorWindows.emplace("Scene", new SceneWindow(*this));
	//_editorWindows.emplace("Game", new GameWindow(*this));
	m_editorWindows.emplace("Properties", new PropertiesWindow(*this));
	m_editorWindows.emplace("Assets", new AssetsWindow(*this));

	set_window_title("");
	Ref<Window> window = WindowManager::get_main();
	window->set_icon("Icon.png");
	window->maximize();

#if NDEBUG
	load_most_recent_project();
#endif // NDEBUG
}

Mintye::EditorApplication::~EditorApplication()
{
	unload_project();

	save_data();

	for (auto const& pair : m_editorWindows)
	{
		delete pair.second;
	}

	MetaDatabase::shutdown();
}

void Mintye::EditorApplication::refresh()
{
	if (mp_project)
	{
		mp_project->refresh();
	}

	for (auto const& [name, window] : m_editorWindows)
	{
		window->refresh();
	}
}

Minty::Int Mintye::EditorApplication::update(Time const& time)
{
	// update threads
	m_taskFactory.update();

	return 0;
}

Int Mintye::EditorApplication::update_gui(Minty::Time const& time)
{
	// load project if able
	if (!mp_project && !m_projectPath.empty())
	{
		load_project(m_projectPath);
		m_projectPath = Path();
	}

	// check for shortcuts made by user
	run_shortcuts();

	int returnCode = 0;

	// draw the GUI
	draw_dock_space();
	returnCode |= draw_menu_bar();
	draw_commands();
	draw_editor_windows();

	return returnCode;
}

void Mintye::EditorApplication::cwd_application() const
{
	std::filesystem::current_path(m_cwd);
}

void Mintye::EditorApplication::cwd_project() const
{
	MINTY_ASSERT(mp_project != nullptr);

	std::filesystem::current_path(mp_project->get_base_path());
}

void Mintye::EditorApplication::load_data()
{
	if (!std::filesystem::exists(DATA_FILENAME)) return;

	PhysicalFile file(DATA_FILENAME, File::Flags::Read);
	TextFileReader reader(&file);
	m_data.deserialize(reader);
}

void Mintye::EditorApplication::save_data() const
{
	PhysicalFile file(DATA_FILENAME, File::Flags::Write);
	TextFileWriter writer(&file);
	m_data.serialize(writer);
}

void Mintye::EditorApplication::set_project(Project* const project)
{
	// set new project
	mp_project = project;

	// set for all windows
	for (auto const& pair : m_editorWindows)
	{
		pair.second->set_project(mp_project);
	}
}

void Mintye::EditorApplication::set_scene(Minty::UUID const id)
{
	// set new scene
	m_sceneId = id;

	// get the scene with the id, or null if empty id
	Ref<Scene> scene = AssetManager::get<Scene>(id);

	// set for all windows
	for (auto const& pair : m_editorWindows)
	{
		pair.second->set_scene(scene);
	}
}

void Mintye::EditorApplication::set_window_title(Minty::String const& subTitle)
{
	Ref<Window> window = WindowManager::get_main();

	if (subTitle.length())
	{
		window->set_title(std::format("{}: {}", NAME, subTitle));
	}
	else
	{
		window->set_title(NAME);
	}
}

void Mintye::EditorApplication::new_project()
{
	GUI::open_popup("Create New Project");
}

void Mintye::EditorApplication::open_project()
{
	GuiFileDialogBuilder builder{};
	builder.path = ".";
	GUI::file_dialog_open("open_project", "Choose project directory...", "", builder);
}

void Mintye::EditorApplication::open_project_directory() const
{
	if (mp_project)
	{
		Operation::open_directory(mp_project->get_build_path());
	}
}

void Mintye::EditorApplication::load_project(Minty::Path const& path)
{
	// if existing project, unload it
	unload_project();

	// load the new one if path is valid
	if (!std::filesystem::exists(path))
	{
		ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");
		console->log_error(std::format("Cannot load project with name \"{}\". It does not exist.", path.string()));
		return;
	}

	Path absPath = std::filesystem::absolute(path);

	// mark as recent
	m_data.emplace_recent_project(absPath);

	// create new project
	Project* project = new Project(absPath);
	project->refresh();

	// create file watcher
	mp_watcher = new FileWatcher(absPath, [this](Path const& path, FileWatcher::FileStatus const status)
		{
			String genericPath = path.generic_string();

			// if anything within the assets folder, re-compile all the assets
			if (genericPath.find(std::format("/{}/", ASSETS_DIRECTORY_NAME)) != std::string::npos)
			{
				m_buildInfo.set_flag(BuildInfo::BuildFlags::Assets);
			}

			//// if anything within the build folder, redo all the project files
			//if (genericPath.find(std::format("/{}/", BUILD_DIRECTORY_NAME)) != std::string::npos)
			//{
			//	_buildInfo.set_flag(BuildInfo::BuildFlags::Program);
			//}

			AssetType type = Asset::get_type_from_path(path);

			// if path was a script file...
			if (type == AssetType::Script)
			{
				// if addition or deletion, regen file
				if (status == FileWatcher::FileStatus::Created || status == FileWatcher::FileStatus::Deleted)
				{
					m_buildInfo.set_flag(BuildInfo::BuildFlags::Assembly);
				}

				// re-build regardless
				m_buildInfo.set_flag(BuildInfo::BuildFlags::AssemblyBuild);
			}

			// if anything changed that would change the application data
			if (
				type == AssetType::Scene && (status == FileWatcher::FileStatus::Created || status == FileWatcher::FileStatus::Deleted)
				)
			{
				m_buildInfo.set_flag(BuildInfo::BuildFlags::ApplicationData);
			}
		});

	// set new types
	set_project(project);
	cwd_project();

	// load assemblies
	Path projectDllPath = get_project_dll_path();
	if (AssetManager::exists(projectDllPath)) 
	{
		// if the DLL exists, copy its meta file into the directory
		Operation::copy(std::format("{}/Assembly.txt", ASSEMBLY_DIRECTORY_NAME), std::format("{}/{}.dll.meta", projectDllPath.parent_path().generic_string(), project->get_name()));

		// load the assembly from there
		ScriptEngine::load_assembly(projectDllPath.stem().string(), projectDllPath);
	}
	else
	{
		MINTY_WARN_FORMAT("No project DLL found at {}.", projectDllPath.generic_string());
	}

	// load project data
	mp_projectData = new EditorProjectData();
	{
		Reader* reader;
		if (AssetManager::open_reader(get_project_editor_path(), reader))
		{
			mp_projectData->deserialize(*reader);

			AssetManager::close_reader(reader);
		}
	}

	// load config for project
	if (!load_config(mp_project->get_minty_path() / DEFAULT_GAME_CONFIG))
	{
		MINTY_ERROR("Failed to load project config.");
	}

	// load a scene, if any found
	Path scenePath = project->find_asset(AssetType::Scene);
	if (!scenePath.empty())
	{
		load_scene(scenePath);
	}

	// set window text to file name
	set_window_title(project->get_name());

	refresh();
}

void Mintye::EditorApplication::unload_project()
{
	unload_scene();

	if (mp_project)
	{
		ScriptEngine::unload_assembly(mp_project->get_name());

		delete mp_projectData;
		mp_projectData = nullptr;

		delete mp_project;
		set_project(nullptr);

		refresh();
	}

	clear_config();

	delete mp_watcher;
	mp_watcher = nullptr;

	cwd_application();
}

void Mintye::EditorApplication::create_new_project(Minty::String const& name, Minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// make a new directory with the name
	Path fullPath = path / name;

	generate_directories(fullPath);

	// done
	console->log(std::format("Created new project: {}", fullPath.string()));
}

Minty::Path Mintye::EditorApplication::get_project_dll_path() const
{
	MINTY_ASSERT(mp_project);

	return Path(std::format("{}/bin/{}/{}.dll", ASSEMBLY_DIRECTORY_NAME, m_buildInfo.get_config_name(), mp_project->get_name()));
}

Minty::Path Mintye::EditorApplication::get_project_editor_path() const
{
	MINTY_ASSERT(mp_project);

	return Path("Editor/editor.minty");
}

void Mintye::EditorApplication::reload_project()
{
	// do nothing if no project loaded
	if (!mp_project) return;

	// automatically reload in update
	m_projectPath = mp_project->get_base_path();
	unload_project();
}

void Mintye::EditorApplication::open_scene()
{
	GuiFileDialogBuilder builder{};
	builder.path = ".";

	GUI::file_dialog_open("open_scene", "Choose scene...", ".scene", builder);
}

void Mintye::EditorApplication::save_scene()
{
	// get and serialize scene
	Ref<Scene> scene = Application::instance().get_scene_manager().get_loaded_scene();

	MINTY_ASSERT_MESSAGE(scene != nullptr, "Cannot save a null Scene.");

	Path scenePath = AssetManager::get_path(scene->id());
	String sceneName = AssetManager::get_name(scene->id());

	if (AssetManager::save(scenePath, scene))
	{
		log(std::format("Saved scene \"{}\".", sceneName));
	}
	else
	{
		log_error(std::format("Failed to save scene \"{}\".", sceneName));
	}
}

void Mintye::EditorApplication::close_scene()
{
	MINTY_ASSERT(m_sceneId.valid());

	unload_scene();
}

void Mintye::EditorApplication::load_scene(Minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error(std::format("Cannot load scene \"{}\". No project loaded.", path.string()));
		return;
	}

	// if none found, do nothing
	if (path.empty())
	{
		console->log_error(std::format("Cannot load scene \"{}\". Project could not find the asset.", path.string()));
	}

	// unload and destroy existing scene
	if (m_sceneId.valid())
	{
		unload_scene();
	}

	// load new scene
	SceneManager& sceneManager = Application::instance().get_scene_manager();
	sceneManager.load(path);
	set_scene(sceneManager.get_loaded_scene()->id());

	refresh();
}

void Mintye::EditorApplication::unload_scene()
{
	if (m_sceneId != INVALID_UUID)
	{
		SceneManager& sceneManager = Application::instance().get_scene_manager();
		sceneManager.unload();
		set_scene(INVALID_UUID);

		refresh();
	}
}

void Mintye::EditorApplication::copy_asset(UUID const id) const
{
	if (m_sceneId.valid())
	{
		Ref<Scene> scene = Application::instance().get_scene_manager().get_loaded_scene();

		MINTY_ASSERT(scene != nullptr);

		Entity entity = scene->get_entity_registry().find_by_id(id);

		if (entity != NULL_ENTITY)
		{
			// ID belongs to entity
			EntityRegistry& registry = scene->get_entity_registry();

			// copy entity to clipboard
			DynamicContainer container;
			TextMemoryWriter writer(&container);
			registry.serialize_entity(writer, entity);
			Operation::set_clipboard_text(std::format("Minty Entity\n{}", to_string(container)).c_str());
			return;
		}
	}

	// not set up yet
	MINTY_TODO("copy_asset for some asset");
}

bool Mintye::EditorApplication::is_asset_copied(Minty::String const& name) const
{
	String clipboard = Operation::get_clipboard_text();

	if (clipboard.empty() || !clipboard.starts_with("Minty ")) return false;

	String type = clipboard.substr(6, clipboard.find_first_of("\n\0", 6) - 6);

	return type == name;
}

void Mintye::EditorApplication::open_asset(Minty::Path const& path)
{
	if (Asset::get_type_from_path(path) == AssetType::Scene)
	{
		// open the scene
		load_scene(path);
	}
	else
	{
		// open in properties
		if (PropertiesWindow* properties = find_editor_window<PropertiesWindow>("Properties"))
		{
			properties->set_target(path);
		}
	}
}

Minty::String Mintye::EditorApplication::get_name(Minty::UUID const id) const
{
	if (mp_project)
	{
		// check assets
		Ref<Asset> asset = AssetManager::get_asset(id);

		if (asset.get())
		{
			return AssetManager::get_name(asset->id());
		}

		// not an asset, check entities
		if (m_sceneId.valid())
		{
			Ref<Scene> scene = Application::instance().get_scene_manager().get_loaded_scene();
			EntityRegistry& registry = scene->get_entity_registry();

			Entity entity = registry.find_by_id(id);

			if (entity != NULL_ENTITY)
			{
				return registry.get_name(entity);
			}
		}
	}

	// none of the above
	return "";
}

void Mintye::EditorApplication::create_asset(Minty::Path const& path)
{
	std::unordered_map<String, String> params
	{
		{ "<NAME>", path.stem().string() }
	};
	create_asset(path, params);
}

void Mintye::EditorApplication::create_directory(Minty::Path const& path)
{
	if (!std::filesystem::create_directory(path))
	{
		if (ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console"))
		{
			console->log_error(std::format("Failed to create a new directory at \"{}\".", path.generic_string()));
		}
	}

	refresh();
}

void Mintye::EditorApplication::create_asset(Minty::Path const& path, std::unordered_map<Minty::String, Minty::String> const& params)
{
	cwd_application();

	String contents = "";

	// if extension given, search for a template with the same extension
	if (path.has_extension())
	{
		Path templatePath = find_template(path.extension());

		if (!templatePath.empty())
		{
			contents = File::read_all_text(templatePath);

			// replace all params in the file
			for (auto const& [key, value] : params)
			{
				// find an instance to replace
				Size pos = contents.find(key);
				while (pos != std::string::npos)
				{
					// replace the found instance
					contents.replace(pos, key.length(), value);

					// find the next instance
					pos = contents.find(key, pos + value.length());
				}
			}
		}
	}

	cwd_project();

	File::write_all_text(path, contents);

	refresh();
}

Minty::Path Mintye::EditorApplication::find_template(Minty::Path const& extension)
{
	// get the name of the file with the given extension
	Minty::Path fileName = Path(std::format("{}.txt", extension.string()));

	Minty::Path templateDirectory = std::filesystem::absolute("Templates");

	// check the templates folder
	for (auto const& entry : std::filesystem::directory_iterator(templateDirectory))
	{
		if (entry.path().filename() == fileName)
		{
			return entry.path();
		}
	}

	// none found
	return Path();
}

void Mintye::EditorApplication::save_project()
{

}

void Mintye::EditorApplication::close_project()
{
	unload_project();
}

void Mintye::EditorApplication::load_most_recent_project()
{
	// if no recents, do nothing
	auto const& recents = m_data.get_recent_projects();
	if (recents.empty()) return;

	Path recent = recents.front();

	// make sure the most recent one exists
	if (!std::filesystem::exists(recent))
	{
		log_error(std::format("Recent project not found: \"{}\".", recent.generic_string()));
		m_data.erase_recent_project(recent);
		return;
	}

	// load it
	load_project(recent);
}

void Mintye::EditorApplication::draw_dock_space()
{
	static bool fullscreen = true;
	static bool padding = false;
	static GuiDockFlags dockspaceFlags = GuiDockFlags::None;

	// set up full screen mode
	GuiWindowFlags windowFlags = GuiWindowFlags::MenuBar | GuiWindowFlags::NoDocking;
	if (fullscreen)
	{
		GuiViewport const& viewport = GUI::get_main_viewport();
		GUI::set_next_window_position(viewport.position);
		GUI::set_next_window_size(viewport.size);
		GUI::set_next_window_viewport(viewport.id);
		GUI::push_style_variable(GuiStyleID::WindowRounding, 0.0f);
		GUI::push_style_variable(GuiStyleID::WindowBorderSize, 0.0f);
		windowFlags |= GuiWindowFlags::NoTitleBar | GuiWindowFlags::NoCollapse | GuiWindowFlags::NoResize | GuiWindowFlags::NoMove | GuiWindowFlags::NoBringToFrontOnFocus | GuiWindowFlags::NoNavFocus | GuiWindowFlags::NoBackground;
	}
	else
	{
		dockspaceFlags &= ~GuiDockFlags::PassthruCentralNode;
	}

	// do not render background if pass thru
	if (static_cast<Bool>(dockspaceFlags & GuiDockFlags::PassthruCentralNode)) windowFlags |= GuiWindowFlags::NoBackground;

	// add (non)padding style
	GUI::push_style_variable(GuiStyleID::WindowPadding, Float2());

	GUI::begin("Dock Space", nullptr, windowFlags);

	// undo padding style
	GUI::pop_style_variable();

	// remove fullscreen styles
	if (fullscreen) GUI::pop_style_variable(2);

	// dock space
	if (static_cast<Bool>(GUI::get_config_flags() & GuiConfigFlags::ImGuiConfigFlags_DockingEnable))
	{
		GUI::dock_space("MyDockSpace", Float2(), dockspaceFlags);
	}

	GUI::end();
}

Int Mintye::EditorApplication::draw_menu_bar()
{
	// dumb work around: cannot open a popup in a menu bar
	// https://github.com/ocornut/imgui/issues/331

	bool createNewProject = false;
	int returnCode = 0;

	if (GUI::begin_main_menu_bar())
	{
		if (GUI::begin_menu("File"))
		{
			if (GUI::menu_item("New Project"))
			{
				createNewProject = true;
			}
			if (GUI::begin_menu("Open Recent...", !m_data.get_recent_projects().empty()))
			{
				// create a menu item for each recent item on the list
				auto const& recents = m_data.get_recent_projects();

				for (auto const& path : recents)
				{
					if (GUI::menu_item(path.generic_string().c_str()))
					{
						load_project(path);
						break;
					}
				}

				GUI::end_menu();
			}
			if (GUI::menu_item("Open Project"))
			{
				open_project();
			}
			if (GUI::menu_item("Save Project", "", nullptr, mp_project))
			{
				save_project();
			}
			if (GUI::menu_item("Close Project", "", nullptr, mp_project))
			{
				close_project();
			}

			GUI::separator();

			if (GUI::menu_item("Open Scene", "Ctrl+O", nullptr, mp_project))
			{
				open_scene();
			}
			if (GUI::menu_item("Save Scene", "Ctrl+S", nullptr, mp_project && m_sceneId.valid()))
			{
				save_scene();
			}
			if (GUI::menu_item("Close Scene", "", nullptr, mp_project && m_sceneId.valid()))
			{
				close_scene();
				returnCode = 1;
			}

			GUI::separator();

			if (GUI::menu_item("Exit"))
			{
				// close the application
				close();
				GUI::end_menu();
				GUI::end_main_menu_bar();
				return 0;
			}

			GUI::end_menu();
		}

		GUI::end_main_menu_bar();
	}

	static char newProjectTitle[64] = "";

	if (createNewProject)
	{
		new_project();
		memset(newProjectTitle, 0, sizeof(newProjectTitle));
	}

	if (GUI::begin_popup_modal("Create New Project"))
	{
		GUI::text("Create New Project");

		if (createNewProject) GUI::set_keyboard_focus_here();

		GUI::input_text("Project Name", newProjectTitle, sizeof(newProjectTitle));

		if (GUI::button("Create") || GUI::is_key_pressed(Key::Enter))
		{
			GuiFileDialogBuilder builder{};
			builder.path = ".";

			GUI::file_dialog_open("new_project", "Choose directory to create project in...", "", builder);

			GUI::close_current_popup();
		}

		GUI::same_line();

		if (GUI::button("Cancel") || GUI::is_key_pressed(Key::Escape))
		{
			GUI::close_current_popup();
		}

		GUI::end_popup();
	}

	// draw open file dialog
	GUI::set_next_window_size(Float2(520.0f, 600.0f), GuiCondition::FirstUseEver);
	if (GUI::file_dialog_display("new_project"))
	{
		if (GUI::file_dialog_is_ok())
		{
			Path filePath = GUI::file_dialog_get_path();

			// create project
			create_new_project(newProjectTitle, filePath);

			// load it also
			load_project(filePath / newProjectTitle);
		}

		// close
		GUI::file_dialog_close();
	}
	GUI::set_next_window_size(Float2(520.0f, 600.0f), GuiCondition::FirstUseEver);
	if (GUI::file_dialog_display("open_project"))
	{
		if (GUI::file_dialog_is_ok())
		{
			//Path filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			Path filePath = GUI::file_dialog_get_path();

			load_project(filePath);
		}

		// close
		GUI::file_dialog_close();
	}
	if (GUI::file_dialog_display("open_scene"))
	{
		if (GUI::file_dialog_is_ok())
		{
			Path filePath = GUI::file_dialog_get_path();
			filePath /= GUI::file_dialog_get_file_name();

			load_scene(filePath);
		}

		// close
		GUI::file_dialog_close();
	}

	return returnCode;
}

void EditorApplication::draw_commands()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// begin new window
	GUI::begin("Commands");

	bool disabled = !mp_project || console->is_command_running();

	if (disabled)
	{
		GUI::begin_disabled();
	}

	// get debug mode
	bool release = m_buildInfo.is_release();
	GUI::checkbox("Release", release);

	// if changed, re-build
	if (release != m_buildInfo.is_release())
	{
		m_buildInfo.set_config(release);
		m_buildInfo.set_flag(BuildInfo::BuildFlags::All);
	}

	// commands:

	if (GUI::button("Clean"))
	{
		clean_project();
	}
	if (GUI::button("Build"))
	{
		build_project();
	}
	if (GUI::button("Run"))
	{
		run_project();
	}
	if (GUI::button("Open"))
	{
		open_project_directory();
	}

	if (disabled)
	{
		GUI::end_disabled();
	}

	GUI::end();
}

void Mintye::EditorApplication::draw_editor_windows()
{
	for (auto const& pair : m_editorWindows)
	{
		pair.second->draw();
	}
}

void Mintye::EditorApplication::reset_editor_windows()
{
	for (auto const& pair : m_editorWindows)
	{
		pair.second->reset();
	}
}

void EditorApplication::generate_cmake()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot generate cmake: no project loaded.");
		return;
	}

	// get path to cmake file
	std::string path = (Path(mp_project->get_build_path()) / "CMakeLists.txt").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(String("Could not open cmake file: ") + path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file <<
		// show generation time TODO: fix this
		//"# " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		// cmake version requirement
		"cmake_minimum_required(VERSION 3.16)" << std::endl <<
		// project name, c++ settings
		"project(" << mp_project->get_name() << " LANGUAGES CXX)" << std::endl <<
		"find_package(Vulkan REQUIRED)" << std::endl <<
		"set(CMAKE_CXX_STANDARD 20)" << std::endl <<
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)" << std::endl <<
		"set(CMAKE_CXX_EXTENSIONS OFF)" << std::endl;

	if (!m_buildInfo.is_release())
	{
		// only ignore if in debug mode
		file << "set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\\\"LIBCMT\\\")" << std::endl;
	}

	Path mintyPath = Operation::get_minty_path();
	String stringMintyPath = mintyPath.generic_string();

	file <<
		// add source files for project
		"set(SOURCES main.cpp)" << std::endl <<
		// make executable with the source files
		"add_executable(${PROJECT_NAME} ${SOURCES})" << std::endl <<
		// set to MT building so it works with the static runtime library
		"set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)" << std::endl <<
		"set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY \"MultiThreaded$<$<CONFIG:Debug>:Debug>\")" << std::endl <<
		// include the runtime dir
		"target_include_directories(${PROJECT_NAME} PRIVATE " << stringMintyPath << "/Runtime/Source PRIVATE " << stringMintyPath << "/Libraries/glm/include PRIVATE " << stringMintyPath << "/Libraries/entt/include PUBLIC ${VULKAN_INCLUDE_DIRS})" << std::endl <<
		// include and link Vulkan
		"include_directories(${Vulkan_INCLUDE_DIRS})" << std::endl <<
		// target and link the MintyRuntime.lib
		"target_link_libraries(${PROJECT_NAME} " << stringMintyPath << "/Runtime/Source/x64/" << m_buildInfo.get_config_name() << "/MintyRuntime.lib)" << std::endl <<
		// target and link the vulkan libs
		"target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})";

	file.close();
}

void EditorApplication::generate_main()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	String path = (Path(mp_project->get_build_path()) / "main.cpp").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		MINTY_ERROR_FORMAT("Could not open main file: {}", path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file <<
		// TODO: fix this
		//"// " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << "\n" <<
		"#include <Minty.h>\n" <<
		"int main(int argc, char const* argv[]) {\n" <<
		"\tMinty::Application app;" << std::endl;

	if (m_buildInfo.is_debug())
	{
		file << "\ttry {" << std::endl;
	}

	file <<
		"\tapp.initialize(Minty::ApplicationBuilder{});\n" <<
		"\tapp.run();" << std::endl;

	if (m_buildInfo.is_debug())
	{
		file <<
			"\t}\n" <<
			"\tcatch (const std::runtime_error& e) {\n" <<
			"\t\tstd::cout << \"Runtime error : \" << e.what() << std::endl;\n" <<
			"\t\tMinty::Debug::log_stack_trace();\n" <<
			"\t}\n" <<
			"\tcatch (const std::exception& e) {\n" <<
			"\t\tstd::cout << \"Exception: \" << e.what() << std::endl;\n" <<
			"\t\tMinty::Debug::log_stack_trace();\n" <<
			"\t}\n" <<
			"\tcatch (...) {\n" <<
			"\t\tstd::cout << \"Unknown exception caught\" << std::endl;\n" <<
			"\t\tMinty::Debug::log_stack_trace();\n" <<
			"\t}\n" << std::endl;
	}

	file <<
		"\treturn 0;\n" <<
		"}" << std::flush;

	file.close();
}

void Mintye::EditorApplication::copy_files()
{
	// copy any DLL's that the Runtime uses

	String configName = m_buildInfo.get_config_name();
	String const& projectName = mp_project->get_name();
	String targetDir = std::format("{}/{}", mp_project->get_build_path().generic_string(), configName);

	// Runtime DLL
	//log_info("\tRuntime DLL");
	//if (!_buildInfo.get_config())
	//{
	//	log_info("\tEngine Debug PDB");
	//	Operation::copy(_cwd / String(MINTY_NAME_ENGINE).append(".pdb"), targetDir);
	//}

	// Mono DLLs
	log_info("\tMono Files");
	Operation::copy(m_cwd / "mono-2.0-sgen.dll", targetDir);
	Operation::copy(m_cwd / "MonoPosixHelper.dll", targetDir);
	Operation::copy(m_cwd / "mscorlib.dll", targetDir);

	// get all dependencies
	std::unordered_set<String> dependencies;

	Ref<ScriptAssembly> assembly = ScriptEngine::get_assembly(mp_project->get_name());
	if (assembly.get())
	{
		std::unordered_set<String> temp = assembly->get_dependencies();
		dependencies.insert(temp.begin(), temp.end());

		// Project DLL
		log_info("\tProject assembly");
		String assemblyPathString = mp_project->get_assembly_path().generic_string();
		String source = std::format("{}/bin/{}/{}.dll", assemblyPathString, configName, projectName);
		Operation::copy(source, targetDir);
		// Project DLL meta
		//String meta = Asset::get_meta_path(source).generic_string();
		String metaSourcePath = std::format("{}/Assembly.txt", assemblyPathString);
		String metaDestinationPath = std::format("{}/{}.dll.meta", targetDir, projectName);
		Operation::copy(metaSourcePath, metaDestinationPath);
	}
	else
	{
		MINTY_WARN("Project assembly not found on build.");
	}

	Ref<ScriptAssembly> engineAssembly = ScriptEngine::get_assembly(MINTY_NAME_ENGINE);
	if (engineAssembly.get())
	{
		std::unordered_set<String> temp = engineAssembly->get_dependencies();
		dependencies.insert(temp.begin(), temp.end());

		// MintyEngine DLL
		log_info("\tEngine assembly");
		Operation::copy(m_cwd / String(MINTY_NAME_ENGINE).append(".dll"), targetDir);
	}
	else
	{
		MINTY_WARN("Engine assembly not found on build.");
	}

	// copy all dependencies
	Operation::copy_some(m_cwd / "mono", Path(targetDir) / "mono", dependencies);

	// Wrap files
	log_info("\tWrap files");
	Operation::copy_files(m_cwd, EXTENSION_WRAP, targetDir);
}

void Mintye::EditorApplication::log(Minty::String const& message)
{
	if (ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console"))
	{
		console->log(message);
	}
	else
	{
		MINTY_LOG(message);
	}
}

void Mintye::EditorApplication::log_warning(Minty::String const& message)
{
	if (ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console"))
	{
		console->log_warning(message);
	}
	else
	{
		MINTY_WARN(message);
	}
}

void Mintye::EditorApplication::log_error(Minty::String const& message)
{
	if (ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console"))
	{
		console->log_error(message);
	}
	else
	{
		MINTY_ERROR(message);
	}
}

void Mintye::EditorApplication::log_info(Minty::String const& message)
{
	if (ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console"))
	{
		console->log_info(message);
	}
	else
	{
		MINTY_INFO(message);
	}
}

void Mintye::EditorApplication::run_shortcuts()
{
	if (GUI::is_key_modifier_pressed(KeyModifiers::Control))
	{
		if (mp_project && m_sceneId.valid() && GUI::is_key_pressed(Key::S, false))
		{
			save_scene();
		}
		if (mp_project && GUI::is_key_pressed(Key::O, false))
		{
			open_scene();
		}
		if (mp_project && GUI::is_key_pressed(Key::R))
		{
			reload_project();
		}
		if (mp_project && GUI::is_key_pressed(Key::B))
		{
			build_project();
		}
	}
	else
	{

	}
}

void Mintye::EditorApplication::load_theme()
{
	Theme theme = GUI::create_theme();
	theme[GuiColorID::Text] = Color(255, 255, 255);
	theme[GuiColorID::TextDisabled] = Color(200, 200, 200);

	theme[GuiColorID::WindowBg] = Color(31, 31, 31);
	theme[GuiColorID::ChildBg] = Color(40, 40, 40);
	theme[GuiColorID::PopupBg] = Color(35, 35, 35);
	theme[GuiColorID::FrameBg] = Color(15, 15, 15);
	theme[GuiColorID::FrameBgHovered] = Color(18, 18, 18);
	theme[GuiColorID::FrameBgActive] = Color(18, 27, 18);

	theme[GuiColorID::TitleBg] = Color(10, 10, 10);
	theme[GuiColorID::TitleBgActive] = Color(43, 118, 43);

	theme[GuiColorID::MenuBarBg] = Color(45, 55, 45);

	theme[GuiColorID::CheckMark] = Color(76, 154, 76);

	theme[GuiColorID::Button] = Color(55, 99, 55);
	theme[GuiColorID::ButtonHovered] = Color(28, 54, 28);
	theme[GuiColorID::ButtonActive] = Color(71, 127, 71);

	theme[GuiColorID::Header] = Color(55, 99, 55);
	theme[GuiColorID::HeaderHovered] = Color(38, 142, 38);
	theme[GuiColorID::HeaderActive] = Color(48, 118, 48);

	theme[GuiColorID::Tab] = Color(45, 83, 45);					// the color of the unselected tab in the focused window
	theme[GuiColorID::TabHovered] = Color(63, 154, 63);			// the color of the tab being hovered over
	theme[GuiColorID::TabActive] = Color(32, 83, 32);				// the color of the selected tab in the focused window
	theme[GuiColorID::TabUnfocused] = Color(20, 28, 20);			// the color of the unselected tab in an unfocused window
	theme[GuiColorID::TabUnfocusedActive] = Color(26, 66, 26);	// the color of the selected tab in an unfocused window

	theme[GuiColorID::DockingPreview] = Color(94, 208, 94, 63);	// color when previewing a location to dock the window in
	theme[GuiColorID::DockingEmptyBg] = Color(5, 5, 5, 0);		// color when nothing in the docked area

	theme[GuiColorID::DragDropTarget] = Color(94, 208, 94, 63);

	GUI::apply_theme(theme);
}

void Mintye::EditorApplication::generate_directory(Minty::Path const& path) const
{
	fs::create_directories(path);
}

void Mintye::EditorApplication::generate_directories(Path const& basePath) const
{
	// create all default project necessary directories
	generate_directory(basePath);

	generate_directory(basePath / ASSETS_DIRECTORY_NAME);
	generate_directory(basePath / BUILD_DIRECTORY_NAME);
	generate_directory(basePath / ASSEMBLY_DIRECTORY_NAME);
	generate_directory(basePath / EDITOR_DIRECTORY_NAME);

	generate_directory(basePath / BUILD_DIRECTORY_NAME / m_buildInfo.get_config_name());
	//generate_directory(basePath / BUILD_DIRECTORY_NAME / "Release");
}

void Mintye::EditorApplication::generate_application_data()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to file
	Path path = mp_project->get_minty_path() / DEFAULT_GAME_APPDATA;

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	Writer* writer;
	if (!AssetManager::open_writer(path, writer))
	{
		MINTY_ERROR_FORMAT("Could not open game application data file: {}", path.string());
		return;
	}

	// initial scene
	// TODO: be able to specify the initial scene manually for each project
	std::set<Path> scenes = mp_project->find_assets(AssetType::Scene);
	Path initialScene = *scenes.begin();

	writer->write("InitialScene", initialScene);
	writer->write("Assemblies", std::vector<String>{
		mp_project->get_name() + ".dll"
	});
	writer->write("Wraps", std::vector<String>{
		"Default.wrap",
		"Assets.wrap"
	});

	AssetManager::close_writer(writer);
}

void Mintye::EditorApplication::refresh_assets()
{
	// ensure copy of collection of asset paths is up to date
	MINTY_ASSERT(mp_project);
	mp_project->refresh();
}

void Mintye::EditorApplication::build_shaders()
{
	// build all vert, frag, glsl shaders within the assets
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");
	MINTY_ASSERT(console);

	MINTY_ASSERT(mp_project);
	std::set<Path> paths = mp_project->find_assets({ ".vert", ".frag", ".glsl" });

	for (auto const& path : paths)
	{
		// build the shader
		String pathString = path.string();
		String command = std::format("glslc {} -o {}.spv", pathString, pathString);
		Int result = system(command.c_str());
	}
}

void Mintye::EditorApplication::generate_wraps()
{
	Path output = mp_project->get_build_path() / m_buildInfo.get_config_name();

	// game data
	// add any files within the Minty directory
	Path mintyPath = mp_project->get_minty_path();
	std::vector<Path> gameWrapFiles;
	for (auto const& entry : std::filesystem::directory_iterator(mintyPath))
	{
		Path const& entryPath = entry.path();

		if (std::filesystem::is_regular_file(entry.status()))
		{
			// get file name
			gameWrapFiles.push_back(entryPath);
		}
	}

	Wrap gameWrap(output / DEFAULT_GAME_WRAP, "Game", static_cast<uint32_t>(gameWrapFiles.size()));
	for (Path const& filePath : gameWrapFiles)
	{
		Path fileName = filePath.filename();

		gameWrap.emplace(filePath, fileName);
	}

	// game assets
	Wrap assetWrap(output / String("Assets").append(EXTENSION_WRAP), ASSETS_DIRECTORY_NAME, static_cast<uint32_t>(mp_project->get_asset_count()), ASSETS_DIRECTORY_NAME);
	mp_project->wrap_assets(assetWrap);

	// built in assets
	std::filesystem::copy("Default.wrap", output / "Default.wrap");
}

void Mintye::EditorApplication::generate_assembly()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	String path = (Path(mp_project->get_assembly_path()) / "Assembly.csproj").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		MINTY_ERROR_FORMAT("Could not open assembly csproj file: {}", path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file
		<< "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl
		<< "<Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << std::endl
		<< "  <Import Project=\"$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props\" Condition=\"Exists('$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props')\" />" << std::endl
		<< "  <PropertyGroup>" << std::endl
		<< "    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>" << std::endl
		<< "    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>" << std::endl
		<< "    <ProjectGuid>{9586B4AD-3861-4962-A65A-9E0C1E5D164B}</ProjectGuid>" << std::endl
		<< "    <OutputType>Library</OutputType>" << std::endl
		<< "    <AppDesignerFolder>Properties</AppDesignerFolder>" << std::endl
		<< "    <RootNamespace></RootNamespace>" << std::endl
		<< "    <AssemblyName>" << mp_project->get_name() << "</AssemblyName>" << std::endl
		<< "    <TargetFrameworkVersion>v4.7.2</TargetFrameworkVersion>" << std::endl
		<< "    <FileAlignment>512</FileAlignment>" << std::endl
		<< "    <Deterministic>true</Deterministic>" << std::endl
		<< "    <TargetFrameworkProfile />" << std::endl
		<< "  </PropertyGroup>" << std::endl
		<< "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)' == 'Debug|x64'\">" << std::endl
		<< "    <DebugSymbols>true</DebugSymbols>" << std::endl
		<< "    <OutputPath>bin\\Debug\\</OutputPath>" << std::endl
		<< "    <DefineConstants>DEBUG;TRACE</DefineConstants>" << std::endl
		<< "    <DebugType>full</DebugType>" << std::endl
		<< "    <PlatformTarget>x64</PlatformTarget>" << std::endl
		<< "    <LangVersion>7.3</LangVersion>" << std::endl
		<< "    <ErrorReport>prompt</ErrorReport>" << std::endl
		<< "  </PropertyGroup>" << std::endl
		<< "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)' == 'Release|x64'\">" << std::endl
		<< "    <OutputPath>bin\\Release\\</OutputPath>" << std::endl
		<< "    <DefineConstants>TRACE</DefineConstants>" << std::endl
		<< "    <Optimize>true</Optimize>" << std::endl
		<< "    <DebugType>pdbonly</DebugType>" << std::endl
		<< "    <PlatformTarget>x64</PlatformTarget>" << std::endl
		<< "    <LangVersion>7.3</LangVersion>" << std::endl
		<< "    <ErrorReport>prompt</ErrorReport>" << std::endl
		<< "  </PropertyGroup>" << std::endl
		<< "  <ItemGroup>" << std::endl
		<< "    <Reference Include=\"" << MINTY_NAME_ENGINE << ", Version=1.0.0.0, Culture=neutral, processorArchitecture=MSIL\">" << std::endl
		<< "      <SpecificVersion>False</SpecificVersion>" << std::endl
		<< "      <HintPath>..\\Build\\" << m_buildInfo.get_config_name() << "\\" << MINTY_NAME_ENGINE << ".dll</HintPath>" << std::endl
		<< "    </Reference>" << std::endl
		<< "    <Reference Include=\"System\" />" << std::endl
		<< "    <Reference Include=\"System.Core\" />" << std::endl
		<< "    <Reference Include=\"System.Xml.Linq\" />" << std::endl
		<< "    <Reference Include=\"System.Data.DataSetExtensions\" />" << std::endl
		<< "    <Reference Include=\"Microsoft.CSharp\" />" << std::endl
		<< "    <Reference Include=\"System.Data\" />" << std::endl
		<< "    <Reference Include=\"System.Net.Http\" />" << std::endl
		<< "    <Reference Include=\"System.Xml\" />" << std::endl
		<< "  </ItemGroup>" << std::endl
		<< "  <ItemGroup>" << std::endl;

	// write all c# file paths
	for (auto const& path : mp_project->find_assets(AssetType::Script))
	{
		file << "    <Compile Include=\"..\\" << path.string() << "\" />" << std::endl;
	}

	file
		<< "  </ItemGroup>" << std::endl
		<< "  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />" << std::endl
		<< "</Project>" << std::endl;

	file.close();

	path = (Path(mp_project->get_assembly_path()) / "Assembly.txt").string();

	// write <project>.dll.meta
	file = std::ofstream(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		MINTY_ERROR_FORMAT("Could not open assembly meta file: {}", path);
		return;
	}

	// write each name with each ID
	Path projectDllPath = get_project_dll_path();
	String projectDllName = projectDllPath.stem().string();
	Ref<ScriptAssembly> assembly = ScriptEngine::get_assembly(projectDllName);

	for (auto const& [id, scriptClass] : *assembly)
	{
		file << scriptClass->get_full_name() << ": " << to_string(id) << std::endl;
	}

	file.close();
}

void EditorApplication::clean_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot clean_project: no project loaded.");
		return;
	}

	console->log_important("clean project");

	Path buildDirectoryPath = mp_project->get_build_path();

	try
	{
		for (auto const& entry : std::filesystem::directory_iterator(buildDirectoryPath))
		{
			std::filesystem::remove_all(entry.path());
		}
	}
	catch (std::filesystem::filesystem_error const& e)
	{
		log_error(std::format("Error cleaning build: {}", e.what()));
	}

	//// clean the build
	//console->run_command("cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build_project . --target clean_project");

	// rebuild everything on next build
	m_buildInfo.set_flag(BuildInfo::BuildFlags::All);
}

void EditorApplication::build_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (m_taskFactory.contains("build"))
	{
		//console->log_error("Cannot build project: last build has not finished.");
		return;
	}

	if (!mp_project)
	{
		console->log_error("Cannot build project: no project loaded.");
		return;
	}

	// check files for changes
	if (mp_watcher)
	{
		mp_watcher->update();
	}

	console->log_important("build project");

	// generate directories before all else
	generate_directories(mp_project->get_base_path());

	// refresh the collection of files
	console->log_important("\trefreshing assets...");
	refresh_assets();

	TaskGroup<void>* taskGroup = m_taskFactory.create("build", [this, console] {
		// done building, remove flags
		m_buildInfo.clear_flags();

		console->log_important("Build finished.");
		});

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::Program))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating cmake...");

			generate_cmake();

			console->log_important("\tgenerating main...");

			generate_main();
			});
	}

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::ApplicationData))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating application data...");

			generate_application_data();
			});
	}

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::Assembly))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating assembly...");

			generate_assembly();
			});
	}

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::Assets))
	{
		taskGroup->create([this, console] {
			console->log_important("\tbuilding shaders...");

			build_shaders();

			console->log_important("\tgenerating wrap files...");

			generate_wraps();
			});
	}

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::Program))
	{
		taskGroup->create([this, console] {
			console->log_important("\tbuilding program...");

			std::string command = "cd " + mp_project->get_build_path().string() + " && " + CMAKE_PATH;

			console->run_commands({
				// create cmake files if needed
				command + " .",
				// build program with cmake
				command + " --build . --config " + m_buildInfo.get_config_name(),
				}, true);
			});
	}

	if (m_buildInfo.get_flag(BuildInfo::BuildFlags::Assembly | BuildInfo::BuildFlags::AssemblyBuild))
	{
		taskGroup->create([this, console] {
			console->log_important("\tbuilding assembly...");

			String assemblyPath = mp_project->get_assembly_path().generic_string();
			String assemblyName = mp_project->get_assembly_path().stem().generic_string();
			String configName = m_buildInfo.get_config_name();

			console->run_commands({
				// build C# assembly
				// https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
				"cd " + assemblyPath + " && dotnet build -c " + configName + " /p:Platform=x64",
				"cd " + assemblyPath + " && copy " + assemblyName + ".dll.meta bin/" + configName + "/"
				}, true);
			});
	}

	// copy files over, always, for now, just to make sure everything is up to date
	taskGroup->create([this, console]
		{
			console->log_important("\tcopying files...");

			copy_files();
		});
}

void EditorApplication::run_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!mp_project)
	{
		console->log_error("Cannot run_project: no project loaded.");
		return;
	}

	console->log_important("run project");

	// call executable, pass in project path as argument for the runtime, so it knows what to run
	console->run_command("cd " + mp_project->get_build_path().string() + " && cd " + m_buildInfo.get_config_name() + " && call " + mp_project->get_name() + ".exe");
}

void Mintye::EditorApplicationData::emplace_recent_project(Minty::Path const& path)
{
	Path newPath = std::filesystem::absolute(path);

	// if the path is in the list, remove and move to front
	for (Size i = 0; i < _recentProjects.size(); i++)
	{
		if (_recentProjects.at(i) == newPath)
		{
			// if not at beginning, move there
			if (i > 0)
			{
				// remove it
				_recentProjects.erase(_recentProjects.begin() + i);

				// add it back
				_recentProjects.insert(_recentProjects.begin(), newPath);
			}

			// done
			return;
		}
	}

	// not in the list, add it
	_recentProjects.insert(_recentProjects.begin(), newPath);

	// if too many, remove the last one
	if (_recentProjects.size() > MAX_RECENT_PROJECTS)
	{
		_recentProjects.pop_back();
	}
}

void Mintye::EditorApplicationData::erase_recent_project(Minty::Path const& path)
{
	// if the path is in the list, remove and move to front
	for (Size i = 0; i < _recentProjects.size(); i++)
	{
		if (_recentProjects.at(i) == path)
		{
			// remove it
			_recentProjects.erase(_recentProjects.begin() + i);

			return;
		}
	}
}

std::vector<Minty::Path> const& Mintye::EditorApplicationData::get_recent_projects() const
{
	return _recentProjects;
}

void Mintye::EditorApplicationData::serialize(Minty::Writer& writer) const
{
	writer.write("recentProjects", _recentProjects);
}

void Mintye::EditorApplicationData::deserialize(Minty::Reader& reader)
{
	reader.read("recentProjects", _recentProjects);
}

void Mintye::EditorProjectData::serialize(Minty::Writer& writer) const
{

}

void Mintye::EditorProjectData::deserialize(Minty::Reader& reader)
{
	if (reader.indent("Scenes"))
	{
		for (Size i = 0; i < reader.size(); i++)
		{
			// get scene path
			String name;
			reader.read_name(i, name);
			Path scenePath = Path(name);

			// read scene data
			EditorSceneData sceneData{};
			reader.indent(i);
			sceneData.deserialize(reader);
			reader.outdent();

			// record data
			m_sceneData[scenePath] = sceneData;
		}

		reader.outdent();
	}
}

void Mintye::EditorSceneData::serialize(Minty::Writer& writer) const
{
}

void Mintye::EditorSceneData::deserialize(Minty::Reader& reader)
{
	reader.read("cameraPosition", m_cameraPosition);
	reader.read("cameraOrientation", m_cameraOrientation);
	reader.read("camera", m_camera);
}
