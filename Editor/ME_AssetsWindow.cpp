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
		get_application().refresh();
	}

	ImGui::SameLine();

	if (ImGui::Button("New File"))
	{
		// new file popup
		ImGui::OpenPopup("Create New Asset File");
	}

	if (ImGui::BeginPopupModal("Create New Asset File"))
	{
		static char newAssetName[255];

		ImGui::Text("Create New Asset File");

		ImGui::SetKeyboardFocusHere();

		ImGui::InputText("Asset Name", newAssetName, IM_ARRAYSIZE(newAssetName));

		if (ImGui::Button("Create") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			// create new file in the currently selected folder, if it does not exist
			Path path = get_project()->get_assets_path() / _path / newAssetName;

			AssetEngine& assets = get_runtime().get_asset_engine();

			if (!assets.exists(path))
			{
				// creating new asset
				File::write_all_text(path, "");

				get_application().refresh();
			}

			memset(newAssetName, 0, IM_ARRAYSIZE(newAssetName));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			memset(newAssetName, 0, IM_ARRAYSIZE(newAssetName));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
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
		if (ImGui::Button(path.string().append("/").c_str(), itemSize))
		{
			// move to new folder
			set_path(_path / path);
			break;
		}
	}

	Scene* scene = get_scene();

	// draw files
	for (auto const& fileData : _files)
	{
		String name = fileData.path.string();

		if (scene && fileData.canIncludeInScene)
		{
			if (fileData.includedInScene)
			{
				name += " [X]";
			}
			else
			{
				name += " [ ]";
			}
		}

		// if clicked, open the file
		if (ImGui::Button(name.c_str(), itemSize))
		{
			EditorApplication& app = get_application();
			app.open_asset(project->get_assets_path() / _path / fileData.path);
		}

		// if right clicked, toggle inclusion in the scene
		if (scene && fileData.canIncludeInScene && ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			// inverse selection
			Scene* scene = get_scene();

			Path projectPath = get_path(fileData.path).lexically_relative(project->get_base_path());

			if (scene->is_registered(projectPath))
			{
				scene->unregister_asset(projectPath);
			}
			else
			{
				scene->register_asset(projectPath);
			}

			// refresh
			get_application().refresh();
		}
	}
	
	ImGui::End();
}

void mintye::AssetsWindow::reset()
{
}

void mintye::AssetsWindow::refresh()
{
	// re-populate the editor files
	set_path(_path);
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

	Scene* scene = get_scene();

	// update directories and paths
	for (auto const& entry : fs::directory_iterator(fullPath))
	{
		// ignore hidden directories (such as .vscode)
		if (fs::is_directory(entry.status()) && !fullPath.stem().string().starts_with("."))
		{
			_directories.push_back(entry.path().stem());
		}
		// add all regular files that aren't meta
		else if (fs::is_regular_file(entry.status()) && Asset::get_type(entry.path()) != AssetType::Meta)
		{
			_files.push_back(FileData{
				.path = entry.path().filename(),
				.canIncludeInScene = Asset::get_type(entry.path()) != AssetType::Scene,
				.includedInScene = scene && scene->is_registered(get_path(entry.path()).lexically_relative(project->get_base_path())),
				});
		}
	}

	int i = 0;
}

minty::Path mintye::AssetsWindow::get_path(minty::Path const& path) const
{
	return get_project()->get_assets_path() / _path / path;
}
