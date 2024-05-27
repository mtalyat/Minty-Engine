#include "ME_EditorApplication.h"

#include "ME_Constants.h"
#include "ME_Project.h"
#include "ME_BuildInfo.h"

#include "ME_EditorApplicationRenderEngine.h"

#include "ME_ConsoleWindow.h"
#include "ME_HierarchyWindow.h"
#include "ME_SceneWindow.h"
#include "ME_GameWindow.h"
#include "ME_PropertiesWindow.h"
#include "ME_AssetsWindow.h"

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
	: Application({
		.renderEngine = new EditorApplicationRenderEngine(*this),
		.mode = ApplicationMode::Edit,
		})
	, _data()
	, _buildInfo()
	, _project()
	, _watcher()
	, _sceneId(INVALID_UUID)
	, _cwd(std::filesystem::current_path())
	, _editorWindows()
{
	// load data
	load_data();

	// create all the editor windows
	// add to list so they get drawn and updated
	_editorWindows.emplace("Console", new ConsoleWindow(*this));
	_editorWindows.emplace("Hierarchy", new HierarchyWindow(*this));
	_editorWindows.emplace("Scene", new SceneWindow(*this));
	//_editorWindows.emplace("Game", new GameWindow(*this));
	_editorWindows.emplace("Properties", new PropertiesWindow(*this));
	_editorWindows.emplace("Assets", new AssetsWindow(*this));

	set_window_title("");
	Window& window = get_window();
	window.set_icon("Icon.png");
	window.maximize();

	load_most_recent_project();
}

Mintye::EditorApplication::~EditorApplication()
{	
	unload_project();

	save_data();

	for (auto const& pair : _editorWindows)
	{
		delete pair.second;
	}
}

void Mintye::EditorApplication::draw()
{
	_taskFactory.update();

	draw_dock_space();
	draw_menu_bar();
	draw_commands();
	draw_editor_windows();
}

void Mintye::EditorApplication::refresh()
{
	if (_project)
	{
		_project->refresh();
	}

	for (auto const& [name, window] : _editorWindows)
	{
		window->refresh();
	}
}

void Mintye::EditorApplication::cwd_application() const
{
	std::filesystem::current_path(_cwd);
}

void Mintye::EditorApplication::cwd_project() const
{
	MINTY_ASSERT(_project != nullptr);

	std::filesystem::current_path(_project->get_base_path());
}

void Mintye::EditorApplication::load_data()
{
	if (!std::filesystem::exists(DATA_FILENAME)) return;

	Node dataNode = File::read_node(DATA_FILENAME);
	Reader reader(dataNode);
	_data.deserialize(reader);
}

void Mintye::EditorApplication::save_data() const
{
	Node dataNode;
	Writer writer(dataNode);
	_data.serialize(writer);
	File::write_node(DATA_FILENAME, dataNode);
}

void Mintye::EditorApplication::set_project(Project* const project)
{
	// set new project
	_project = project;

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_project(_project);
	}
}

void Mintye::EditorApplication::set_scene(Minty::UUID const id)
{
	// set new scene
	_sceneId = id;

	// get the scene with the id, or null if empty id
	DefaultLayer& layer = get_default_layer();
	SceneManager& sceneManager = layer.get_scene_manager();
	Ref<Scene> scene = sceneManager.get_scene(id);

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_scene(scene);
	}
}

void Mintye::EditorApplication::set_window_title(Minty::String const& subTitle)
{
	Window& window = get_window();

	if (subTitle.length())
	{
		window.set_title(std::format("{}: {}", NAME, subTitle));
	}
	else
	{
		window.set_title(NAME);
	}
}

void Mintye::EditorApplication::new_project()
{
	ImGui::OpenPopup("Create New Project");
}

void Mintye::EditorApplication::open_project()
{
	IGFD::FileDialogConfig config
	{
		.path = "."
	};
	ImGuiFileDialog::Instance()->OpenDialog("open_project", "Choose project directory...", nullptr, config);
}

void Mintye::EditorApplication::load_project(Minty::Path const& path)
{
	// if existing project, unload it
	unload_project();

	// load the new one if path is valid
	if (!std::filesystem::exists(path))
	{
		ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");
		console->log_error(std::format("Cannot load project with path \"{}\". It does not exist.", path.string()));
		return;
	}

	Path absPath = std::filesystem::absolute(path);

	// mark as recent
	_data.emplace_recent_project(absPath);

	// create new project
	Project* project = new Project(absPath);
	project->refresh();

	// create file watcher
	_watcher = new FileWatcher(absPath, [this](Path const& path, FileWatcher::FileStatus const status)
		{
			String genericPath = path.generic_string();

			// if anything within the assets folder, re-compile all the assets
			if (genericPath.find(std::format("/{}/", ASSETS_DIRECTORY_NAME)) != std::string::npos)
			{
				_buildInfo.set_flag(BuildInfo::BuildFlags::Assets);
			}

			//// if anything within the build folder, redo all the project files
			//if (genericPath.find(std::format("/{}/", BUILD_DIRECTORY_NAME)) != std::string::npos)
			//{
			//	_buildInfo.set_flag(BuildInfo::BuildFlags::Program);
			//}

			AssetType type = Asset::get_type(path);

			// if path was a script file...
			if (type == AssetType::Script)
			{
				// if addition or deletion, regen file
				if (status == FileWatcher::FileStatus::Created || status == FileWatcher::FileStatus::Deleted)
				{
					_buildInfo.set_flag(BuildInfo::BuildFlags::Assembly);
				}

				// re-build regardless
				_buildInfo.set_flag(BuildInfo::BuildFlags::AssemblyBuild);
			}

			// if anything changed that would change the application data
			if (
				type == AssetType::Scene && (status == FileWatcher::FileStatus::Created || status == FileWatcher::FileStatus::Deleted)
				)
			{
				_buildInfo.set_flag(BuildInfo::BuildFlags::ApplicationData);
			}
		});

	// set new types
	set_project(project);
	cwd_project();

	// load assemblies
	ScriptEngine::instance().load_assembly(std::format("{}/bin/Debug/{}.dll", ASSEMBLY_DIRECTORY_NAME, project->get_name()));

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

	if (_project)
	{
		delete _project;
		set_project(nullptr);

		refresh();
	}

	MINTY_DELETE(_watcher);

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

void Mintye::EditorApplication::open_scene()
{
	IGFD::FileDialogConfig config
	{
		.path = "."
	};
	ImGuiFileDialog::Instance()->OpenDialog("open_scene", "Choose scene...", ".scene", config);
}

void Mintye::EditorApplication::save_scene()
{
	MINTY_ASSERT(_sceneId.valid());

	// get and serialize scene
	Ref<Scene> scene = Application::instance().get_default_layer().get_scene_manager().at_scene(_sceneId);
	Node node("", to_string(_sceneId));
	SerializationData data
	{
		.scene = scene.get(),
		.entity = NULL_ENTITY,
	};
	Writer writer(node, &data);
	scene->serialize(writer);

	if (File::write_node(scene->get_path(), node))
	{
		log(std::format("Saved scene \"{}\".", scene->get_name()));
	}
	else
	{
		log_error(std::format("Failed to save scene \"{}\".", scene->get_name()));
	}
}

void Mintye::EditorApplication::close_scene()
{
	MINTY_ASSERT(_sceneId.valid());

	unload_scene();
}

void Mintye::EditorApplication::load_scene(Minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
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
	if (_sceneId.valid())
	{
		unload_scene();
	}

	// load new scene
	SceneManager& sceneManager = Application::instance().get_scene_manager();
	set_scene(sceneManager.create_scene(path).get_id());
	sceneManager.load_scene(_sceneId);
	sceneManager.load();

	refresh();
}

void Mintye::EditorApplication::unload_scene()
{
	if (_sceneId != INVALID_UUID)
	{
		SceneManager& sceneManager = Application::instance().get_scene_manager();
		sceneManager.unload();
		sceneManager.destroy();
		set_scene(INVALID_UUID);

		refresh();
	}
}

void Mintye::EditorApplication::copy_asset(UUID const id) const
{
	if (_sceneId.valid())
	{
		Ref<Scene> scene = Application::instance().get_scene_manager().get_scene(_sceneId);

		MINTY_ASSERT(scene != nullptr);

		Entity entity = scene->get_entity_registry().find_by_id(id);

		if (entity != NULL_ENTITY)
		{
			// ID belongs to entity
			EntityRegistry& registry = scene->get_entity_registry();

			// copy entity to clipboard
			Node node = registry.serialize_entity(entity);
			String text = node.get_formatted_string();
			ImGui::SetClipboardText(std::format("Minty Entity\n{}", text).c_str());
			return;
		}
	}

	// not set up yet
	MINTY_TODO("copy_asset for some asset");
}

bool Mintye::EditorApplication::is_asset_copied(Minty::String const& name) const
{
	if (!ImGui::GetClipboardText()) return false;

	String clipboard = ImGui::GetClipboardText();

	if (clipboard.empty() || !clipboard.starts_with("Minty ")) return false;

	String type = clipboard.substr(6, clipboard.find_first_of("\n\0", 6) - 6);

	return type == name;
}

void Mintye::EditorApplication::open_asset(Minty::Path const& path)
{
	if (Asset::get_type(path) == AssetType::Scene)
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
	if (_project)
	{
		// check assets
		AssetEngine& assets = AssetEngine::instance();

		if (Ref<Asset> asset = assets.get_asset(id))
		{
			return asset->get_name();
		}

		// not an asset, check entities
		if (_sceneId.valid())
		{
			Ref<Scene> scene = Application::instance().get_scene_manager().at_scene(_sceneId);
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
				size_t pos = contents.find(key);
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
	auto const& recents = _data.get_recent_projects();
	if (recents.empty()) return;

	Path recent = recents.front();

	// make sure the most recent one exists
	if (!std::filesystem::exists(recent))
	{
		log_error(std::format("Recent project not found: \"{}\".", recent.generic_string()));
		_data.erase_recent_project(recent);
		return;
	}

	// load it
	load_project(recent);
}

void Mintye::EditorApplication::draw_dock_space()
{
	static bool fullscreen = true;
	static bool padding = false;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	// set up full screen mode
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (fullscreen)
	{
		ImGuiViewport const* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// do not render background if pass thru
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) windowFlags |= ImGuiWindowFlags_NoBackground;

	// add (non)padding style
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dock Space", nullptr, windowFlags);

	// undo padding style
	ImGui::PopStyleVar();

	// remove fullscreen styles
	if (fullscreen) ImGui::PopStyleVar(2);

	// dock space
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	ImGui::End();
}

void Mintye::EditorApplication::draw_menu_bar()
{
	// dumb work around: cannot open a popup in a menu bar
	// https://github.com/ocornut/imgui/issues/331

	bool createNewProject = false;

	bool ctrl = ImGui::GetIO().KeyCtrl;

	if (_project && _sceneId.valid() && ctrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S), false))
	{
		save_scene();
	}
	if (_project && ctrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O), false))
	{
		open_scene();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				createNewProject = true;
			}
			if (ImGui::BeginMenu("Open Recent...", !_data.get_recent_projects().empty()))
			{
				// create a menu item for each recent item on the list
				auto const& recents = _data.get_recent_projects();

				for (auto const& path : recents)
				{
					if (ImGui::MenuItem(path.generic_string().c_str()))
					{
						load_project(path);
						break;
					}
				}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open Project"))
			{
				open_project();
			}
			if (ImGui::MenuItem("Save Project", nullptr, nullptr, _project))
			{
				save_project();
			}
			if (ImGui::MenuItem("Close Project", nullptr, nullptr, _project))
			{
				close_project();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open Scene", "Ctrl+O", nullptr, _project))
			{
				open_scene();
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S", nullptr, _project && _sceneId.valid()))
			{
				save_scene();
			}
			if (ImGui::MenuItem("Close Scene", nullptr, nullptr, _project && _sceneId.valid()))
			{
				close_scene();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				std::exit(0);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	static char newProjectTitle[64] = "";

	if (createNewProject)
	{
		new_project();
		memset(newProjectTitle, 0, IM_ARRAYSIZE(newProjectTitle));
	}

	if (ImGui::BeginPopupModal("Create New Project"))
	{
		ImGui::Text("Create New Project");

		if (createNewProject) ImGui::SetKeyboardFocusHere();

		ImGui::InputText("Project Name", newProjectTitle, IM_ARRAYSIZE(newProjectTitle));

		if (ImGui::Button("Create") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			IGFD::FileDialogConfig config
			{
				.path = "."
			};
			ImGuiFileDialog::Instance()->OpenDialog("new_project", "Choose directory to create project in...", nullptr, config);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// draw open file dialog
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (ImGuiFileDialog::Instance()->Display("new_project"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			Path filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			// create project
			create_new_project(newProjectTitle, filePath);

			// load it also
			load_project(filePath / newProjectTitle);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (ImGuiFileDialog::Instance()->Display("open_project"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			//Path filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			Path filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			load_project(filePath);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("open_scene"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			Path filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			filePath /= ImGuiFileDialog::Instance()->GetCurrentFileName();

			load_scene(filePath);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void EditorApplication::draw_commands()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// begin new window
	ImGui::Begin("Commands");

	bool disabled = !_project || console->is_command_running();

	if (disabled)
	{
		ImGui::BeginDisabled();
	}

	// get debug mode
	bool release = _buildInfo.get_config();
	ImGui::Checkbox("Release", &release);

	// if changed, re-build
	if (release != _buildInfo.get_config())
	{
		_buildInfo.set_config(release);
		_buildInfo.set_flag(BuildInfo::BuildFlags::All);
	}

	// commands:

	if (ImGui::Button("Clean"))
	{
		clean_project();
	}
	if (ImGui::Button("Build"))
	{
		build_project();
	}
	if (ImGui::Button("Run"))
	{
		run_project();
	}

	if (disabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void Mintye::EditorApplication::draw_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->draw();
	}
}

void Mintye::EditorApplication::reset_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->reset();
	}
}

void EditorApplication::generate_cmake()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate cmake: no project loaded.");
		return;
	}

	// get path to cmake file
	std::string path = (Path(_project->get_build_path()) / "CMakeLists.txt").string();

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
		// show generation time
		"# " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		// cmake version requirement
		"cmake_minimum_required(VERSION 3.16)" << std::endl <<
		// project name, c++ settings
		"project(" << _project->get_name() << " LANGUAGES CXX)" << std::endl <<
		"find_package(Vulkan REQUIRED)" << std::endl <<
		"set(CMAKE_CXX_STANDARD 20)" << std::endl <<
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)" << std::endl <<
		"set(CMAKE_CXX_EXTENSIONS OFF)" << std::endl;

	if (!_buildInfo.get_config())
	{
		// only ignore if in debug mode
		file << "set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\\\"LIBCMT\\\")" << std::endl;
	}

	Path MintyPath = Operations::get_minty_path();
	String stringMintyPath = MintyPath.generic_string();

	file <<
		// add source files for project
		"set(SOURCES main.cpp)" << std::endl <<
		// make executable with the source files
		"add_executable(${PROJECT_NAME} ${SOURCES})" << std::endl <<
		// set to MT building so it works with the static runtime library
		"set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)" << std::endl <<
		"set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY \"MultiThreaded$<$<CONFIG:Debug>:Debug>\")" << std::endl <<
		// include the runtime dir
		"target_include_directories(${PROJECT_NAME} PRIVATE C:/Users/mitch/source/repos/Minty-Engine/Runtime PUBLIC ${VULKAN_INCLUDE_DIRS})" << std::endl <<
		// copy any DLL's that the Runtime uses
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/mono-2.0-sgen.dll ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << "/mono-2.0-sgen.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/MonoPosixHelper.dll ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << "/MonoPosixHelper.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/mscorlib.dll ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << "/mscorlib.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy " << stringMintyPath << "/Libraries/MintyEngine/bin/" << _buildInfo.get_config_name() << "/MintyEngine.dll ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << "/MintyEngine.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ../" << ASSEMBLY_DIRECTORY_NAME << "/bin/" << _buildInfo.get_config_name() << "/" << _project->get_name() << ".dll ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << "/" << _project->get_name() << ".dll)" << std::endl <<
		// copy all necessary engine data files
		"file(GLOB DATA_FILES \"C:/Users/mitch/source/repos/Minty-Engine/Data/*.wrap\")" << std::endl <<
		"file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/" << _buildInfo.get_config_name() << ")" << std::endl <<
		// include and link Vulkan
		"include_directories(${Vulkan_INCLUDE_DIRS})" << std::endl <<
		// target and link the MintyRuntime.lib
		"target_link_libraries(${PROJECT_NAME} C:/Users/mitch/source/repos/Minty-Engine/Runtime/x64/" << _buildInfo.get_config_name() << "/MintyRuntime.lib)" << std::endl <<
		// target and link the vulkan libs
		"target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})";

	file.close();
}

void EditorApplication::generate_main()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	String path = (Path(_project->get_build_path()) / "main.cpp").string();

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
		"// " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		"#include <Minty.h>" << std::endl <<
		"int main(int argc, char const* argv[]) {" << std::endl <<
		"\tMinty::Application app;" << std::endl <<
		"\tapp.run();" << std::endl <<
		"\treturn 0;" << std::endl <<
		"}";

	file.close();
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

	generate_directory(basePath / BUILD_DIRECTORY_NAME / _buildInfo.get_config_name());
	//generate_directory(basePath / BUILD_DIRECTORY_NAME / "Release");
}

void Mintye::EditorApplication::generate_application_data()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to file
	Path path = _project->get_build_path() / String("game").append(EXTENSION_APPLICATION_DATA);

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		MINTY_ERROR_FORMAT("Could not open game application data file: {}", path.string());
		return;
	}

	// TODO: use Writer, save the node

	// write contents
	file <<
		"scenes" << std::endl;

	// write all scenes
	std::set<Path> scenes = _project->find_assets(AssetType::Scene);
	for (Path const& scenePath : scenes)
	{
		file << "\t- " << scenePath.string() << std::endl;
	}

	file <<
		"assemblies" << std::endl;

	// assemblies copied by cmake
	std::vector<String> assemblies =
	{
		"MintyEngine.dll",
		std::format("{}.dll", _project->get_name()),
	};

	for (auto const& str : assemblies)
	{
		file << "\t- " << str << std::endl;
	}

	file.close();
}

void Mintye::EditorApplication::generate_wraps()
{
	// TODO: split into multile wrap files if needed

	Path output = _project->get_build_path() / _buildInfo.get_config_name();

	// for now:
	// compile all game files/assets into two wrap files

	// game data
	Wrap gameWrap(output / String("game").append(EXTENSION_WRAP), "Game", 1);
	String appFileName = String("game").append(EXTENSION_APPLICATION_DATA);
	Path appPath = _project->get_build_path() / appFileName;
	gameWrap.emplace(appPath, appFileName);

	// game assets
	Wrap assetWrap(output / String("assets").append(EXTENSION_WRAP), ASSETS_DIRECTORY_NAME, static_cast<uint32_t>(_project->get_asset_count()), ASSETS_DIRECTORY_NAME);
	_project->wrap_assets(assetWrap);
}

void Mintye::EditorApplication::generate_assembly()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	String path = (Path(_project->get_assembly_path()) / "Assembly.csproj").string();

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

	Path MintyPath = Operations::get_minty_path();
	String stringMintyPath = MintyPath.string();

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
		<< "    <AssemblyName>" << _project->get_name() << "</AssemblyName>" << std::endl
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
		<< "    <Reference Include=\"MintyEngine, Version=1.0.0.0, Culture=neutral, processorArchitecture=MSIL\">" << std::endl
		<< "      <SpecificVersion>False</SpecificVersion>" << std::endl
		<< "      <HintPath>" << stringMintyPath << "\\Libraries\\MintyEngine\\bin\\Debug\\MintyEngine.dll</HintPath>" << std::endl
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
	//<< "    <Compile Include=\"..\Assets\Scripts\CameraController.cs\" />" << std::endl
	//<< "    <Compile Include=\"..\Assets\Scripts\Link.cs\" />" << std::endl
	//<< "    <Compile Include=\"..\Assets\Scripts\PlayerController.cs\" />" << std::endl
	//<< "    <Compile Include=\"..\Assets\Scripts\Session.cs\" />" << std::endl
	//<< "    <Compile Include=\"..\Assets\Scripts\TestScript.cs\" />" << std::endl
	//<< "    <Compile Include=\"Properties\AssemblyInfo.cs\" />" << std::endl

// write all c# file paths
	for (auto const& path : _project->find_assets(AssetType::Script))
	{
		file << "    <Compile Include=\"..\\" << path.string() << "\" />" << std::endl;
	}

	file
		<< "  </ItemGroup>" << std::endl
		<< "  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />" << std::endl
		<< "</Project>" << std::endl;

	file.close();
}

void EditorApplication::clean_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot clean_project: no project loaded.");
		return;
	}

	console->log_important("clean project");

	Path buildDirectoryPath = _project->get_build_path();

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
	_buildInfo.set_flag(BuildInfo::BuildFlags::All);
}

void EditorApplication::build_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (_taskFactory.contains("build"))
	{
		console->log_error("Cannot build project: last build has not finished.");
		return;
	}

	if (!_project)
	{
		console->log_error("Cannot build project: no project loaded.");
		return;
	}

	// check files for changes
	if (_watcher)
	{
		_watcher->update();
	}

	console->log_important("build project");

	// generate directories before all else
	generate_directories(_project->get_base_path());

	TaskGroup<void>* taskGroup = _taskFactory.create("build", [this, console] {
		// done building, remove flags
		_buildInfo.clear_flags();

		console->log_important("Build finished.");
		});

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::Program))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating cmake...");

			generate_cmake();

			console->log_important("\tgenerating main...");

			generate_main();
			});
	}

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::ApplicationData))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating application data...");

			generate_application_data();
			});
	}

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::Assembly))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating assembly...");

			generate_assembly();
			});
	}

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::Assets))
	{
		taskGroup->create([this, console] {
			console->log_important("\tgenerating wrap files...");

			generate_wraps();
			});
	}

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::Program))
	{
		taskGroup->create([this, console] {
			console->log_important("\tbuilding program...");

			std::string command = "cd " + _project->get_build_path().string() + " && " + CMAKE_PATH;

			console->run_commands({
				// create cmake files if needed
				command + " .",
				// build program with cmake
				command + " --build . --config " + _buildInfo.get_config_name(),
				}, true);
			});
	}

	if (_buildInfo.get_flag(BuildInfo::BuildFlags::Assembly | BuildInfo::BuildFlags::AssemblyBuild))
	{
		taskGroup->create([this, console] {
			console->log_important("\tbuilding assembly...");

			String assemblyPath = _project->get_assembly_path().generic_string();
			String configName = _buildInfo.get_config_name();

			console->run_commands({
				// build C# assembly
				// https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
				"cd " + assemblyPath + " && dotnet build -c " + configName + " /p:Platform=x64"
				}, true);

			// if not running cmake build, but scripts rebuilt, copy the DLL over manually, since the cmake is not doing it
			if (_buildInfo.get_flag(BuildInfo::BuildFlags::Assembly | BuildInfo::BuildFlags::AssemblyBuild) && !_buildInfo.get_flag(BuildInfo::BuildFlags::Program))
			{
				console->log_info("Manually copying files");

				String configName = _buildInfo.get_config_name();
				String const& projectName = _project->get_name();
				String source = std::format("{}/bin/{}/{}.dll", _project->get_assembly_path().generic_string(), configName, projectName);
				String destination = std::format("{}/{}/{}.dll", _project->get_build_path().generic_string(), configName, projectName);

				try {
					std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
				}
				catch (std::filesystem::filesystem_error& e)
				{
					console->log_error(std::format("Failed to copy assembly DLL: \"{}\"", e.what()));
				}
			}
			});
	}
}

void EditorApplication::run_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot run_project: no project loaded.");
		return;
	}

	console->log_important("run project");

	// call executable, pass in project path as argument for the runtime, so it knows what to run
	console->run_command("cd " + _project->get_build_path().string() + " && cd " + _buildInfo.get_config_name() + " && call " + _project->get_name() + ".exe");
}

Mintye::EditorApplicationData::EditorApplicationData()
	: Minty::Object()
	, _recentProjects()
{}

void Mintye::EditorApplicationData::emplace_recent_project(Minty::Path const& path)
{
	Path newPath = std::filesystem::absolute(path);

	// if the path is in the list, remove and move to front
	for (size_t i = 0; i < _recentProjects.size(); i++)
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
	for (size_t i = 0; i < _recentProjects.size(); i++)
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

void Mintye::EditorApplicationData::deserialize(Minty::Reader const& reader)
{
	reader.read_vector("recentProjects", _recentProjects);
}
