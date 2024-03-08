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

namespace fs = std::filesystem;

#define CMAKE_PATH "C:/Users/mitch/source/repos/Minty-Engine/Editor/cmake/bin/cmake.exe"
#define EditorApplication_NAME "TestProject"
#define EXE_NAME std::string(EditorApplication_NAME).append(".exe")

using namespace mintye;
using namespace minty;

EditorApplication::EditorApplication()
	: Application()
	, _project()
	, _sceneId(ERROR_ID)
	, _editorWindows()
{
	// create all the editor windows
	// add to list so they get drawn and updated
	_editorWindows.emplace("Console", new ConsoleWindow(*this));
	_editorWindows.emplace("Hierarchy", new HierarchyWindow(*this));
	_editorWindows.emplace("Scene", new SceneWindow(*this));
	//_editorWindows.emplace("Game", new GameWindow(*this));
	_editorWindows.emplace("Properties", new PropertiesWindow(*this));
	_editorWindows.emplace("Assets", new AssetsWindow(*this));
}

mintye::EditorApplication::~EditorApplication()
{
	for(auto const& pair : _editorWindows)
	{
		delete pair.second;
	}
}

void mintye::EditorApplication::init(RuntimeBuilder* b)
{
	RuntimeBuilder builder
	{
		.renderEngine = new EditorApplicationRenderEngine(*this, get_runtime()),
	};

	Application::init(&builder);

	// init the window
	set_window_title("");
	Window& window = get_window();
	window.maximize();

	load_assemblies({ "../Libraries/MintyEngine/bin/x64/Debug/MintyEngine.dll" });

	// TODO: remove this
	// if the TestProject exists, open it by default
	if (std::filesystem::exists("../Projects/Tests/TestProject"))
	{
		load_project("../Projects/Tests/TestProject");
	}
}

void EditorApplication::loop()
{
	Application::loop();
}

void mintye::EditorApplication::destroy()
{
	unload_project();

	reset_editor_windows();

	Application::destroy();
}

void mintye::EditorApplication::draw()
{
	BuildInfo buildInfo
	{
		.debug = true
	};

	draw_dock_space();
	draw_menu_bar();
	draw_commands(buildInfo);
	draw_editor_windows();
}

minty::Runtime* mintye::EditorApplication::create_runtime()
{
	return new Runtime(get_info(), RunMode::Edit);
}

minty::Window* mintye::EditorApplication::create_window()
{
	return new Window("", 1280, 720, "Icon.png");
}

void mintye::EditorApplication::set_project(Project* const project)
{
	// set new project
	_project = project;

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_project(_project);
	}
}

void mintye::EditorApplication::set_scene(minty::UUID const id)
{
	// set new scene
	_sceneId = id;

	// get the scene with the id, or null if empty id
	Scene* scene = get_runtime().get_scene_manager().get_scene(id);

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_scene(scene);
	}
}

void mintye::EditorApplication::set_window_title(minty::String const& subTitle)
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

void mintye::EditorApplication::new_project()
{
	ImGui::OpenPopup("Create New Project");
}

void mintye::EditorApplication::open_project()
{
	IGFD::FileDialogConfig config
	{
		.path = "."
	};
	ImGuiFileDialog::Instance()->OpenDialog("open_project", "Choose project directory...", nullptr, config);
}

void mintye::EditorApplication::load_project(minty::Path const& path)
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

	// create new project
	Project* project = new Project(path);
	project->collect_assets();
	std::filesystem::current_path(path);

	// set new types
	set_project(project);

	// load assemblies
	// C:\Users\mitch\source\repos\Minty-Engine\Projects\Tests\TestProject\Assembly\bin\Debug
	get_runtime().get_script_engine().load_assembly(std::format("{0}/bin/x64/Debug/{0}.dll", project->get_name()));

	// load a scene, if any found
	Path scenePath = project->find_asset(Project::CommonFileType::Scene);
	if (!scenePath.empty())
	{
		load_scene(scenePath);
	}

	// set window text to file name
	set_window_title(project->get_name());
}

void mintye::EditorApplication::unload_project()
{
	unload_scene();

	if (_project)
	{
		delete _project;
		set_project(nullptr);
	}
}

void mintye::EditorApplication::create_new_project(minty::String const& name, minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// make a new directory with the name
	Path fullPath = path / name;

	// create new project directory
	if (!fs::create_directory(fullPath))
	{
		console->log_error(std::format("Failed to create project at path: {}", fullPath.string()));
		return;
	}

	// create all default project necessary directories

	if (!fs::create_directory(fullPath / ASSETS_DIRECTORY_NAME))
	{
		console->log_error(std::format("Failed to create Assets folder in project path: {}", fullPath.string()));
		return;
	}
	if (!fs::create_directory(fullPath / BUILD_DIRECTORY_NAME))
	{
		console->log_error(std::format("Failed to create Build folder in project path: {}", fullPath.string()));
		return;
	}

	// done
	console->log(std::format("Created new project: {}", fullPath.string()));
}

void mintye::EditorApplication::load_scene(minty::Path const& path)
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
	SceneManager& sceneManager = get_runtime().get_scene_manager();
	set_scene(sceneManager.create_scene(path).get_id());
	sceneManager.load_scene(_sceneId);
	sceneManager.load();
}

void mintye::EditorApplication::unload_scene()
{
	if (_sceneId != INVALID_UUID)
	{
		SceneManager& sceneManager = get_runtime().get_scene_manager();
		sceneManager.unload();
		sceneManager.destroy();
		set_scene(INVALID_UUID);
	}
}

void mintye::EditorApplication::open_asset(minty::Path const& path)
{
	Path extension = path.extension();

	if (extension == SCENE_EXTENSION)
	{
		// open the scene
		load_scene(path);
	}
	else
	{
		// cannot open in the editor, open on the OS
		minty::Operations::open(path);
	}
}

void mintye::EditorApplication::save_project()
{
	minty::Console::todo("EditorEditorApplication::save_project()");
}

void mintye::EditorApplication::save_as_project()
{
	minty::Console::todo("EditorEditorApplication::save_as_project()");
}

void mintye::EditorApplication::close_project()
{
	unload_project();
}

void mintye::EditorApplication::draw_dock_space()
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

void mintye::EditorApplication::draw_menu_bar()
{
	// dumb work around: cannot open a popup in a menu bar
	// https://github.com/ocornut/imgui/issues/331

	bool createNewProject = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				createNewProject = true;
			}
			if (ImGui::MenuItem("Open Project")) // "Ctrl+O"
			{
				open_project();
			}
			if (ImGui::MenuItem("Save Project", nullptr, false, false)) // "Ctrl+S", disabled for now
			{
				save_project();
			}
			if (ImGui::MenuItem("Save Project as...", nullptr, false, false)) // disabled for now
			{
				save_as_project();
			}
			if (ImGui::MenuItem("Close Project", nullptr, false, _project))
			{
				close_project();
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

	if (createNewProject)
	{
		new_project();
	}

	static char newProjectTitle[64] = "";

	if (ImGui::BeginPopupModal("Create New Project"))
	{
		ImGui::Text("Create New Project");

		ImGui::InputText("Project Name", newProjectTitle, IM_ARRAYSIZE(newProjectTitle));

		if (ImGui::Button("Create"))
		{
			IGFD::FileDialogConfig config
			{
				.path = "."
			};
			ImGuiFileDialog::Instance()->OpenDialog("new_project", "Choose directory to create project in...", nullptr, config);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
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
}

void EditorApplication::draw_commands(BuildInfo& buildInfo)
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
	ImGui::Checkbox("Debug", &buildInfo.debug);

	// commands:

	if (ImGui::Button("Clean"))
	{
		clean_project();
	}
	if (ImGui::Button("Build"))
	{
		build_project(buildInfo);
	}
	if (ImGui::Button("Run"))
	{
		run_project(buildInfo);
	}

	if (disabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void mintye::EditorApplication::draw_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->draw();
	}
}

void mintye::EditorApplication::reset_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->reset();
	}
}

void EditorApplication::generate_cmake(BuildInfo const& buildInfo)
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
		"project(" << EditorApplication_NAME << " LANGUAGES CXX)" << std::endl <<
		"find_package(Vulkan REQUIRED)" << std::endl <<
		"set(CMAKE_CXX_STANDARD 20)" << std::endl <<
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)" << std::endl <<
		"set(CMAKE_CXX_EXTENSIONS OFF)" << std::endl;

	if (buildInfo.debug)
	{
		// only ignore if in debug mode
		file << "set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\\\"LIBCMT\\\")" << std::endl;
	}

	// get all local paths for source files
	std::stringstream pathsStream;
	minty::Path buildPath = _project->get_build_path();
	for (minty::Path const& path : _project->find_assets(Project::CommonFileType::Source))
	{
		pathsStream << " " << std::filesystem::relative(ASSETS_DIRECTORY_NAME / path, buildPath).generic_string();
	}

	file <<
		// add source files for project
		"set(SOURCES main.cpp" << pathsStream.str() << ")" << std::endl <<
		// make executable with the source files
		"add_executable(${PROJECT_NAME} ${SOURCES})" << std::endl <<
		// set to MT building so it works with the static runtime library
		"set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)" << std::endl <<
		"set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY \"MultiThreaded$<$<CONFIG:Debug>:Debug>\")" << std::endl <<
		// include the runtime dir
		"target_include_directories(${PROJECT_NAME} PRIVATE C:/Users/mitch/source/repos/Minty-Engine/Runtime PUBLIC ${VULKAN_INCLUDE_DIRS})" << std::endl <<
		// copy any DLL's that the Runtime uses
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/mono-2.0-sgen.dll ${CMAKE_CURRENT_BINARY_DIR}/" << buildInfo.get_config() << "/mono-2.0-sgen.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/MonoPosixHelper.dll ${CMAKE_CURRENT_BINARY_DIR}/" << buildInfo.get_config() << "/MonoPosixHelper.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy C:/Libraries/Mono/lib/mscorlib.dll ${CMAKE_CURRENT_BINARY_DIR}/" << buildInfo.get_config() << "/mscorlib.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ../" << _project->get_name() << "/bin/x64/" << buildInfo.get_config() << "/MintyEngine.dll ${CMAKE_CURRENT_BINARY_DIR}/" << buildInfo.get_config() << "/MintyEngine.dll)" << std::endl <<
		"add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ../" << _project->get_name() << "/bin/x64/" << buildInfo.get_config() << "/" << _project->get_name() << ".dll ${CMAKE_CURRENT_BINARY_DIR}/" << buildInfo.get_config() << "/" << _project->get_name() << ".dll)" << std::endl <<
		// include and link Vulkan
		"include_directories(${Vulkan_INCLUDE_DIRS})" << std::endl <<
		// target and link the MintyRuntime.lib
		"target_link_libraries(${PROJECT_NAME} C:/Users/mitch/source/repos/Minty-Engine/Runtime/x64/" << buildInfo.get_config() << "/MintyRuntime.lib)" << std::endl <<
		// target and link the vulkan libs
		"target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})";

	file.close();
}

void EditorApplication::generate_main(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	std::string path = (std::filesystem::path(_project->get_build_path()) / "main.cpp").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		minty::Console::error(std::string("Could not open main file: ") + path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	Info const& projectInfo = _project->get_info();

	// write contents
	file <<
		"// " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		"#include <Minty.h>" << std::endl <<
		"int main(int argc, char const* argv[]) {" << std::endl <<
		"\tminty::Application app;" << std::endl <<
		"\treturn app.run();" << std::endl <<
		"}";

	file.close();
}

void mintye::EditorApplication::generate_directories()
{

}

void mintye::EditorApplication::generate_application_data(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	Path path = _project->get_build_path() / String("game").append(APPLICATION_EXTENSION);

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		minty::Console::error(std::format("Could not open main file: {}", path.string()));
		return;
	}

	Info const& projectInfo = _project->get_info();

	// TODO: use Writer, save the node

	// write contents
	file <<
		"scenes" << std::endl;

	// write all scenes
	std::set<Path> scenes = _project->find_assets(Project::CommonFileType::Scene);
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

void EditorApplication::clean_project()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot clean_project: no project loaded.");
		return;
	}

	console->log_important("clean project");

	// clean the build
	console->run_command("cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build_project . --target clean_project");
}

void EditorApplication::build_project(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot build project: no project loaded.");
		return;
	}

	console->log_important("build project");

	generate_directories();

	console->log_important("\tgenerating cmake...");

	generate_cmake(buildInfo);

	console->log_important("\tgenerating main...");

	generate_main(buildInfo);

	console->log_important("\tgenerating application data...");

	generate_application_data(buildInfo);

	console->log_important("\tgenerating wrap files...");

	generate_wraps(buildInfo);

	console->log_important("\tbuilding program...");

	std::string command = "cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string();
	console->run_commands({
		// make cmake files if needed
		command + " .",
		// build program
		command + " --build . --config " + buildInfo.get_config(),
		});
}

void EditorApplication::run_project(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot run_project: no project loaded.");
		return;
	}

	console->log_important("run project");

	// call executable, pass in project path as argument for the runtime, so it knows what to run
	console->run_command("cd " + _project->get_build_path().string() + " && cd " + buildInfo.get_config() + " && call " + EXE_NAME);
}

void mintye::EditorApplication::generate_wraps(BuildInfo const& buildInfo)
{
	// TODO: split into multile wrap files if needed

	Path output = _project->get_build_path() / buildInfo.get_config();

	// for now:
	// compile all game files/assets into two wrap files

	// game data
	Wrap gameWrap(output / String("game").append(WRAP_EXTENSION), "Game", 1);
	String appFileName = String("game").append(APPLICATION_EXTENSION);
	Path appPath = _project->get_build_path() / appFileName;
	gameWrap.emplace(appPath, appFileName);

	// game assets
	Wrap assetWrap(output / String("assets").append(WRAP_EXTENSION), ASSETS_DIRECTORY_NAME, static_cast<uint32_t>(_project->get_asset_count()), ASSETS_DIRECTORY_NAME);
	_project->wrap_assets(assetWrap);
}
