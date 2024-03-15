#include "ME_AssetsWindow.h"

#include "ME_EditorApplication.h"
#include "ME_Project.h"
#include "ME_ConsoleWindow.h"
#include <filesystem>

using namespace minty;
using namespace mintye;
namespace fs = std::filesystem;

mintye::AssetsWindow::AssetsWindow(EditorApplication& application)
	: EditorWindow(application)
	, _path()
	, _directories()
	, _files()
{
	// go to Assets folder
	set_path("");
}

void mintye::AssetsWindow::set_project(Project* const project)
{
	// set the project
	EditorWindow::set_project(project);

	// update the path to the project's Assets folder
	set_path("");
}

void mintye::AssetsWindow::draw()
{
	Project* project = get_project();

	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

	// do not draw if no project loaded
	if (!ImGui::Begin("Assets") || !project) 
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Refresh"))
	{
		// refresh the project files
		get_project()->refresh();

		// re-populate the editor files
		set_path(_path);
	}

	ImGui::SameLine();

	if (ImGui::Button("Open Folder"))
	{
		// open the assets folder
		Operations::open(project->get_assets_path() / _path);
	}

	ImGui::Separator();

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 itemSize(windowSize.x, 30.0f);

	// draw a list of buttons with their paths
	// always draw back
	if (!_path.empty() && ImGui::Button("../", itemSize))
	{
		// go back
		if (!_path.has_parent_path())
		{
			// go back to Assets
			set_path("");
		}
		else
		{
			// go to previous folder
			set_path(_path.parent_path());
		}
	}

	// draw directories
	for (auto const& path : _directories)
	{
		// if clicked, move to new directory
		if (ImGui::Button(path.string().c_str(), itemSize))
		{
			// move to new folder
			set_path(_path / path);
			break;
		}
	}

	// draw files
	for (auto const& path : _files)
	{
		// if clicked, open the file
		if (ImGui::Button(path.string().c_str(), itemSize))
		{
			EditorApplication& app = get_application();
			app.open_asset(project->get_assets_path() / _path / path);
			//minty::Operations::open(project->get_assets_path() / _path / path);
		}
	}
	
	ImGui::End();
}

void mintye::AssetsWindow::reset()
{
}

void mintye::AssetsWindow::set_path(minty::Path const& path)
{
	// set the new path
	_path = path;

	// clear old data
	_directories.clear();
	_files.clear();

	Project* project = get_project();

	// do nothing if no project loaded
	if (!project) return;

	Path fullPath = project->get_assets_path() / _path;

	// update directories and paths
	for (auto const& entry : fs::directory_iterator(fullPath))
	{
		// ignore hidden directories (such as .vscode)
		if (fs::is_directory(entry.status()) && !fullPath.stem().string().starts_with("."))
		{
			_directories.push_back(entry.path().stem());
		}
		// add all regular files that aren't meta
		else if (fs::is_regular_file(entry.status()) && entry.path().extension() != META_EXTENSION)
		{
			_files.push_back(entry.path().filename());
		}
	}
}
