#include "ME_Application.h"

#include "ME_Constants.h"
#include "ME_Project.h"
#include "ME_BuildInfo.h"

#include "ME_ApplicationRenderEngine.h"

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
#define APPLICATION_NAME "TestProject"
#define EXE_NAME std::string(APPLICATION_NAME).append(".exe")

using namespace mintye;
using namespace minty;

Application::Application()
	: _info(NAME, 0, 0, 0)
	, _path(std::filesystem::current_path())
	, _window("", 1280, 720, "Icon.png")
	, _runtime(new Runtime(_info, Runtime::Mode::Edit))
	, _project()
	, _sceneId(ERROR_ID)
	, _editorWindows()
{
	// init the window
	set_window_title("");
	_window.maximize();

	ApplicationRenderEngine* renderEngine = new ApplicationRenderEngine(*this, *_runtime);

	// init the runtime
	RuntimeBuilder runtimeBuilder {
		.window = &_window,
		.renderEngine = renderEngine,
	};
	_runtime->init(&runtimeBuilder);

	// init the render engine
	RenderEngineBuilder renderEngineBuilder{
		.info = &_info,
		.window = &_window
	};
	_runtime->get_render_engine().init(renderEngineBuilder);

	// create all the editor windows
	// add to list so they get drawn and updated
	_editorWindows.emplace("Console", new ConsoleWindow(*this));
	_editorWindows.emplace("Hierarchy", new HierarchyWindow(*this));
	//_editorWindows.emplace("Scene", new SceneWindow(*this));
	//_editorWindows.emplace("Game", new GameWindow(*this));
	_editorWindows.emplace("Properties", new PropertiesWindow(*this));
	_editorWindows.emplace("Assets", new AssetsWindow(*this));

	// load the engine and editor assemblies
	ScriptEngine& scriptEngine = _runtime->get_script_engine();
	scriptEngine.load_assembly("../Libraries/MintyEngine/bin/x64/Debug/MintyEngine.dll");
	//scriptEngine.load_assembly("../Libraries/MintyEditor/bin/x64/Debug/MintyEditor.dll");
	_runtime->link();
}

mintye::Application::~Application()
{
	for(auto const& pair : _editorWindows)
	{
		delete pair.second;
	}

	if (_runtime)
	{
		// stop if needed
		if (_runtime->is_running())
		{
			_runtime->stop();
			_runtime->cleanup();
		}

		// cleanup and destroy
		_runtime->destroy();
		delete _runtime;
	}
}

int Application::run(int argc, char const* argv[])
{
	// TODO: remove this
	// if the TestProject exists, open it by default
	if (std::filesystem::exists("../Projects/Tests/TestProject"))
	{
		load_project("../Projects/Tests/TestProject");
	}

	_runtime->start();

	_runtime->run();

	_runtime->cleanup();
	cleanup();

	return 0;
}

minty::Runtime& mintye::Application::get_runtime() const
{
	return *_runtime;
}

void mintye::Application::draw()
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

void mintye::Application::cleanup()
{
	unload_scene();
	unload_project();
	reset_editor_windows();
}

void mintye::Application::set_project(Project* const project)
{
	// set new project
	_project = project;

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_project(_project);
	}
}

void mintye::Application::set_scene(minty::UUID const id)
{
	// set new scene
	_sceneId = id;

	// get the scene with the id, or null if empty id
	Scene* scene = _runtime->get_scene_manager().get_scene(id);

	// set for all windows
	for (auto const& pair : _editorWindows)
	{
		pair.second->set_scene(scene);
	}
}

void mintye::Application::set_window_title(minty::String const& subTitle)
{
	if (subTitle.length())
	{
		_window.set_title(std::format("{}: {}", NAME, subTitle));
	}
	else
	{
		_window.set_title(NAME);
	}
}

void mintye::Application::new_project()
{
	ImGui::OpenPopup("Create New Project");
}

void mintye::Application::open_project()
{
	IGFD::FileDialogConfig config
	{
		.path = "."
	};
	ImGuiFileDialog::Instance()->OpenDialog("open_project", "Choose project directory...", nullptr, config);
}

void mintye::Application::load_project(minty::Path const& path)
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
	_runtime->get_script_engine().load_assembly(std::format("{0}/bin/x64/Debug/{0}.dll", project->get_name()));

	// load a scene, if any found
	Path scenePath = project->find_asset(Project::CommonFileType::Scene);
	if (!scenePath.empty())
	{
		load_scene(scenePath);
	}

	// set window text to file name
	set_window_title(project->get_name());
}

void mintye::Application::unload_project()
{
	unload_scene();

	if (_project)
	{
		delete _project;
		set_project(nullptr);
	}

	// reset path
	std::filesystem::current_path(_path);
}

void mintye::Application::create_new_project(minty::String const& name, minty::Path const& path, NewProjectSetupType initType)
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

	// create all default project necessary files
	generate_init(fullPath);

	// init other folders/files if needed
	switch (initType)
	{
	case NewProjectSetupType::Default:
		break;
	case NewProjectSetupType::VSCode:
	{
		// create directory
		if (!fs::create_directory(fullPath / VSCODE_DIRECTORY_NAME))
		{
			console->log_error(std::format("Failed to create .vscode folder in project path: {}", fullPath.string()));
			return;
		}

		// create files
		generate_vscode();
	}
		break;
	}

	// done
	console->log(std::format("Created new project: {}", fullPath.string()));
}

void mintye::Application::load_scene(minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error(std::format("Cannot load scene \"{}\". No project loaded.", path.string()));
		return;
	}

	if (!_runtime)
	{
		console->log_error(std::format("Cannot load scene \"{}\". No engine loaded.", path.string()));
		return;
	}

	// if none found, do nothing
	if (path.empty())
	{
		console->log_error(std::format("Cannot load scene \"{}\". Project could not find the asset.", path.string()));
	}

	// unload existing scene
	unload_scene();

	// load new scene
	SceneManager& sceneManager = _runtime->get_scene_manager();
	set_scene(sceneManager.create_scene(path).get_id());
	sceneManager.load_scene(_sceneId);
	sceneManager.load();
}

void mintye::Application::unload_scene()
{
	if (_sceneId != INVALID_UUID)
	{
		SceneManager& sceneManager = _runtime->get_scene_manager();
		sceneManager.unload();
		sceneManager.destroy();
		set_scene(INVALID_UUID);
	}
}

void mintye::Application::open_asset(minty::Path const& path)
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

void mintye::Application::save_project()
{
	minty::Console::todo("Application::save_project()");
}

void mintye::Application::save_as_project()
{
	minty::Console::todo("Application::save_as_project()");
}

void mintye::Application::close_project()
{
	unload_project();
}

void mintye::Application::draw_dock_space()
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

void mintye::Application::draw_menu_bar()
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
	static NewProjectSetupType newProjectSetupType = NewProjectSetupType::Default;

	if (ImGui::BeginPopupModal("Create New Project"))
	{
		ImGui::Text("Create New Project");

		ImGui::InputText("Project Name", newProjectTitle, IM_ARRAYSIZE(newProjectTitle));

		static int currentItem = 0;
		char const* const items[] = { "Default", "VSCode" };
		int itemsCount = sizeof(items) / sizeof(*items);

		if (ImGui::Combo("Setup Type", &currentItem, items, itemsCount))
		{
			newProjectSetupType = static_cast<NewProjectSetupType>(currentItem);
		}

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
			create_new_project(newProjectTitle, filePath, newProjectSetupType);

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

void Application::draw_commands(BuildInfo& buildInfo)
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
		clean();
	}
	if (ImGui::Button("Build"))
	{
		build(buildInfo);
	}
	if (ImGui::Button("Run"))
	{
		run(buildInfo);
	}

	if (disabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void mintye::Application::draw_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->draw();
	}
}

void mintye::Application::reset_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->reset();
	}
}

void Application::generate_cmake(BuildInfo const& buildInfo)
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
		"project(" << APPLICATION_NAME << " LANGUAGES CXX)" << std::endl <<
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
		// include and link Vulkan
		"include_directories(${Vulkan_INCLUDE_DIRS})" << std::endl <<
		// target and link the MintyRuntime.lib
		"target_link_libraries(${PROJECT_NAME} C:/Users/mitch/source/repos/Minty-Engine/Runtime/x64/" << buildInfo.get_config() << "/MintyRuntime.lib)" << std::endl <<
		// target and link the vulkan libs
		"target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})";

	file.close();
}

void Application::generate_main(BuildInfo const& buildInfo)
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
		"#include <string>" << std::endl <<
		"#include <format>" << std::endl <<
		"#include <filesystem>" << std::endl <<
		"#include \"../Assets/init.h\"" << std::endl <<
		"int main(int argc, char const* argv[]) {" << std::endl <<
		"	std::filesystem::current_path(\"" << _project->get_base_path().generic_string() << "\");" << std::endl << // move out of Build/Debug or Build/Release folder, into base folder
		"	minty::Console::log(std::filesystem::current_path().string());" << std::endl <<
		"	minty::Info info(\"" << projectInfo.get_application_name() << "\", " << projectInfo.get_application_major() << ", " << projectInfo.get_application_minor() << ", " << projectInfo.get_application_patch() << ");" << std::endl <<
		"	minty::Runtime runtime(info);" << std::endl;
	
	if (buildInfo.debug)
	{
		file << "	try {" << std::endl;
	}

	file <<
		"	runtime.init();" << std::endl <<
		"	runtime.get_script_engine().load_assembly(\"" << projectInfo.get_application_name() << "/bin/x64/" << buildInfo.get_config() << "/MintyEngine.dll\");" << std::endl <<
		"	runtime.get_script_engine().load_assembly(\"" << projectInfo.get_application_name() << "/bin/x64/" << buildInfo.get_config() << "/" << projectInfo.get_application_name() << ".dll\");" << std::endl <<
		"	runtime.link();" << std::endl <<
		"	if(int code = init(runtime)) { minty::Console::error(std::format(\"Failed to init program with error code {}.\", code)); return code; }" << std::endl <<
		"	runtime.start();" << std::endl <<
		"	runtime.run();" << std::endl <<
		"	runtime.cleanup();" << std::endl <<
		"	if(int code = destroy(runtime)) { minty::Console::error(std::format(\"Failed to destroy program with error code {}.\", code)); return code; }" << std::endl <<
		"	runtime.destroy();" << std::endl;

	if (buildInfo.debug)
	{
		file << "	} catch (std::exception const& e) { minty::Console::error(std::format(\"Crash: \\\"{}\\\"\", e.what())); }" << std::endl;
	}

	file << 
		"	return runtime.get_exit_code();" << std::endl <<
		"}";

	file.close();
}

void mintye::Application::generate_vscode()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate vscode: no project loaded.");
		return;
	}

	// get path to files file
	Path vsCodePath = _project->get_sub_path(VSCODE_DIRECTORY_NAME);
	String cppPropertiesPath = (vsCodePath / "c_cpp_properties.json").string();
	String launchPath = (vsCodePath / "launch.json").string();
	String settingsPath = (vsCodePath / "settings.json").string();

	// open file to overwrite
	std::ofstream file(cppPropertiesPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open c_cpp_properties.json file: {}", cppPropertiesPath));
		return;
	}
	
	file
		<< "{" << std::endl
		<< "    \"configurations\": [" << std::endl
		<< "        {" << std::endl
		<< "            \"name\": \"Win32\"," << std::endl
		<< "            \"includePath\": [" << std::endl
		<< "                \"${workspaceFolder}/**\"," << std::endl
		<< "                \"C:\\\\Users\\\\mitch\\\\source\\\\repos\\\\Minty-Engine\\\\Runtime\"," << std::endl
		<< "                \"${Vulkan_INCLUDE_DIR}\"" << std::endl
		<< "            ]," << std::endl
		<< "            \"defines\": [" << std::endl
		<< "                \"_DEBUG\"," << std::endl
		<< "                \"UNICODE\"," << std::endl
		<< "                \"_UNICODE\"" << std::endl
		<< "            ]," << std::endl
		<< "            \"windowsSdkVersion\": \"10.0.22621.0\"," << std::endl
		<< "            \"compilerPath\": \"cl.exe\"," << std::endl
		<< "            \"cStandard\": \"c17\"," << std::endl
		<< "            \"cppStandard\": \"c++20\"," << std::endl
		<< "            \"intelliSenseMode\": \"windows-msvc-x64\"" << std::endl
		<< "        }" << std::endl
		<< "    ]," << std::endl
		<< "    \"version\": 4" << std::endl
		<< "}" << std::endl;

	console->log_warning("c_cpp_properties.json: hard coded cmake.sourceDirectory");

	file.close();

	file = std::ofstream(launchPath, std::ios::trunc);

	if (!file.is_open())
	{
		console->log_error(std::format("Could not open launch.json file: {}", launchPath));
		return;
	}

	file
		<< "{" << std::endl
		<< "    \"version\": \"0.2.0\"," << std::endl
		<< "    \"configurations\": [" << std::endl
		<< "        {" << std::endl
		<< "            \"name\": \"(msvc) Launch\"," << std::endl
		<< "            \"type\": \"cppvsdbg\"," << std::endl
		<< "            \"request\": \"launch\"," << std::endl
		<< "            // Resolved by CMake Tools:" << std::endl
		<< "            \"program\": \"${command:cmake.launchTargetPath}\"," << std::endl
		<< "            \"args\": []," << std::endl
		<< "            \"stopAtEntry\": false," << std::endl
		<< "            \"cwd\": \"${workspaceFolder}\"," << std::endl
		<< "            \"environment\": [" << std::endl
		<< "                {" << std::endl
		<< "                    // add the directory where our target was built to the PATHs" << std::endl
		<< "                    // it gets resolved by CMake Tools:" << std::endl
		<< "                    \"name\": \"PATH\"," << std::endl
		<< "                    \"value\": \"${env:PATH}:${command:cmake.getLaunchTargetDirectory}\"" << std::endl
		<< "                }" << std::endl
		<< "            ]," << std::endl
		<< "            \"console\": \"internalConsole\"" << std::endl
		<< "        }" << std::endl
		<< "    ]" << std::endl
		<< "}" << std::endl;

	file.close();

	file = std::ofstream(settingsPath, std::ios::trunc);

	if (!file.is_open())
	{
		console->log_error(std::format("Could not open settings.json file: {}", settingsPath));
		return;
	}

	file
		<< "{" << std::endl
		<< "    \"files.associations\": {" << std::endl
		<< "        \"ostream\": \"cpp\"," << std::endl
		<< "        \"algorithm\": \"cpp\"," << std::endl
		<< "        \"array\": \"cpp\"," << std::endl
		<< "        \"atomic\": \"cpp\"," << std::endl
		<< "        \"bit\": \"cpp\"," << std::endl
		<< "        \"cctype\": \"cpp\"," << std::endl
		<< "        \"charconv\": \"cpp\"," << std::endl
		<< "        \"chrono\": \"cpp\"," << std::endl
		<< "        \"clocale\": \"cpp\"," << std::endl
		<< "        \"cmath\": \"cpp\"," << std::endl
		<< "        \"compare\": \"cpp\"," << std::endl
		<< "        \"concepts\": \"cpp\"," << std::endl
		<< "        \"cstddef\": \"cpp\"," << std::endl
		<< "        \"cstdint\": \"cpp\"," << std::endl
		<< "        \"cstdio\": \"cpp\"," << std::endl
		<< "        \"cstdlib\": \"cpp\"," << std::endl
		<< "        \"cstring\": \"cpp\"," << std::endl
		<< "        \"ctime\": \"cpp\"," << std::endl
		<< "        \"cwchar\": \"cpp\"," << std::endl
		<< "        \"deque\": \"cpp\"," << std::endl
		<< "        \"exception\": \"cpp\"," << std::endl
		<< "        \"filesystem\": \"cpp\"," << std::endl
		<< "        \"format\": \"cpp\"," << std::endl
		<< "        \"forward_list\": \"cpp\"," << std::endl
		<< "        \"fstream\": \"cpp\"," << std::endl
		<< "        \"functional\": \"cpp\"," << std::endl
		<< "        \"initializer_list\": \"cpp\"," << std::endl
		<< "        \"iomanip\": \"cpp\"," << std::endl
		<< "        \"ios\": \"cpp\"," << std::endl
		<< "        \"iosfwd\": \"cpp\"," << std::endl
		<< "        \"iostream\": \"cpp\"," << std::endl
		<< "        \"istream\": \"cpp\"," << std::endl
		<< "        \"iterator\": \"cpp\"," << std::endl
		<< "        \"limits\": \"cpp\"," << std::endl
		<< "        \"list\": \"cpp\"," << std::endl
		<< "        \"locale\": \"cpp\"," << std::endl
		<< "        \"map\": \"cpp\"," << std::endl
		<< "        \"memory\": \"cpp\"," << std::endl
		<< "        \"mutex\": \"cpp\"," << std::endl
		<< "        \"new\": \"cpp\"," << std::endl
		<< "        \"optional\": \"cpp\"," << std::endl
		<< "        \"ratio\": \"cpp\"," << std::endl
		<< "        \"set\": \"cpp\"," << std::endl
		<< "        \"sstream\": \"cpp\"," << std::endl
		<< "        \"stdexcept\": \"cpp\"," << std::endl
		<< "        \"stop_token\": \"cpp\"," << std::endl
		<< "        \"streambuf\": \"cpp\"," << std::endl
		<< "        \"string\": \"cpp\"," << std::endl
		<< "        \"system_error\": \"cpp\"," << std::endl
		<< "        \"thread\": \"cpp\"," << std::endl
		<< "        \"tuple\": \"cpp\"," << std::endl
		<< "        \"type_traits\": \"cpp\"," << std::endl
		<< "        \"typeinfo\": \"cpp\"," << std::endl
		<< "        \"unordered_map\": \"cpp\"," << std::endl
		<< "        \"unordered_set\": \"cpp\"," << std::endl
		<< "        \"utility\": \"cpp\"," << std::endl
		<< "        \"vector\": \"cpp\"," << std::endl
		<< "        \"xfacet\": \"cpp\"," << std::endl
		<< "        \"xhash\": \"cpp\"," << std::endl
		<< "        \"xiosbase\": \"cpp\"," << std::endl
		<< "        \"xlocale\": \"cpp\"," << std::endl
		<< "        \"xlocbuf\": \"cpp\"," << std::endl
		<< "        \"xlocinfo\": \"cpp\"," << std::endl
		<< "        \"xlocmes\": \"cpp\"," << std::endl
		<< "        \"xlocmon\": \"cpp\"," << std::endl
		<< "        \"xlocnum\": \"cpp\"," << std::endl
		<< "        \"xloctime\": \"cpp\"," << std::endl
		<< "        \"xmemory\": \"cpp\"," << std::endl
		<< "        \"xstring\": \"cpp\"," << std::endl
		<< "        \"xtr1common\": \"cpp\"," << std::endl
		<< "        \"xtree\": \"cpp\"," << std::endl
		<< "        \"xutility\": \"cpp\"," << std::endl
		<< "        \"regex\": \"cpp\"" << std::endl
		<< "    }," << std::endl
		<< "    \"liveServer.settings.port\": 5501," << std::endl
		<< "    \"cmake.sourceDirectory\": \"C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject/Build\"," << std::endl
		<< "    \"cmake.loggingLevel\": \"debug\"," << std::endl
		<< "}" << std::endl;

	console->log_warning("settings.json: hard coded cmake.sourceDirectory");

	file.close();
}

void mintye::Application::generate_init(minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// get path to files file
	Path assetsPath = path / ASSETS_DIRECTORY_NAME;
	Path hPath = assetsPath / INIT_H_NAME;
	Path cppPath = assetsPath / INIT_CPP_NAME;

	// open file to overwrite
	std::ofstream file(hPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open {} file: {}", INIT_H_NAME, hPath.string()));
		return;
	}

	file
		<< "#pragma once" << std::endl
		<< "" << std::endl
		<< "#include <Minty.h>" << std::endl
		<< "" << std::endl
		<< "int init(minty::Engine& engine);" << std::endl
		<< "int destroy(minty::Engine& engine);" << std::endl;

	file.close();

	// open file to overwrite
	file = std::ofstream(cppPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open {} file: {}", INIT_CPP_NAME, cppPath.string()));
		return;
	}

	file
		<< "#include \"init.h\"" << std::endl
		<< "" << std::endl
		<< "using namespace minty;" << std::endl
		<< "" << std::endl
		<< "int init(Engine &engine)" << std::endl
		<< "{" << std::endl
		<< "    return 0;" << std::endl
		<< "}" << std::endl
		<< "" << std::endl
		<< "int destroy(Engine& engine)" << std::endl
		<< "{" << std::endl
		<< "    return 0;" << std::endl
		<< "}" << std::endl;

	file.close();
}

void Application::clean()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot clean: no project loaded.");
		return;
	}

	console->log_important("clean");

	// clean the build
	console->run_command("cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build . --target clean");
}

void Application::build(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot build: no project loaded.");
		return;
	}

	console->log_important("build");

	console->log_important("\tgenerating cmake");

	generate_cmake(buildInfo);

	console->log_important("\tgenerating main");

	generate_main(buildInfo);

	console->log_important("\tbuilding program");

	std::string command = "cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string();
	console->run_commands({
		// // make cmake files if needed
		command + " .",
		// build program
		command + " --build . --config " + buildInfo.get_config(),
		});
}

void Application::run(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot run: no project loaded.");
		return;
	}

	console->log_important("run");

	// call executable, pass in project path as argument for the runtime, so it knows what to run
	console->run_command("cd " + _project->get_build_path().string() + " && cd " + buildInfo.get_config() + " && call " + EXE_NAME + " " + _project->get_base_path().string());
}