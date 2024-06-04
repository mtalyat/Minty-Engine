#include "ME_AssetsWindow.h"

#include "ME_EditorApplication.h"
#include "ME_Project.h"
#include "ME_ConsoleWindow.h"
#include "ME_ImGuiHelper.h"
#include <filesystem>

using namespace Minty;
using namespace Mintye;
namespace fs = std::filesystem;

Mintye::AssetsWindow::AssetsWindow(EditorApplication& application)
	: EditorWindow(application)
	, _path()
	, _directories()
	, _files()
{
	// go to base folder
	set_path("");
}

void Mintye::AssetsWindow::set_project(Project* const project)
{
	// set the project
	EditorWindow::set_project(project);

	// update the path to the project's base folder
	set_path("");
}

void Mintye::AssetsWindow::draw()
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

	bool inBuiltInDirectory = _path.string().starts_with("BuiltIn");

	static bool popupOpen = false;

	if (!inBuiltInDirectory && ImGui::Button("New File"))
	{
		// new file popup
		ImGui::OpenPopup("Create New Asset File");
		popupOpen = true;
	}

	if (ImGui::BeginPopupModal("Create New Asset File"))
	{
		static char newAssetName[255];

		ImGui::Text("Create New Asset File");

		if (popupOpen)
		{
			ImGui::SetKeyboardFocusHere();
			popupOpen = false;
		}

		ImGui::InputText("Asset Name", newAssetName, IM_ARRAYSIZE(newAssetName));

		if (ImGui::Button("Create") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			// create new file in the currently selected folder, if it does not exist
			Path path = get_project()->get_base_path() / _path / newAssetName;

			AssetEngine& assets = AssetEngine::instance();

			if (!assets.exists(path))
			{
				// creating new asset
				get_application().create_asset(path);
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

	if (!inBuiltInDirectory && ImGui::Button("New Directory"))
	{
		// new file popup
		ImGui::OpenPopup("Create New Asset Directory");
		popupOpen = true;
	}

	if (ImGui::BeginPopupModal("Create New Asset Directory"))
	{
		static char newDirectoryName[255];

		ImGui::Text("Create New Asset Directory");

		if (popupOpen)
		{
			ImGui::SetKeyboardFocusHere();
			popupOpen = false;
		}

		ImGui::InputText("Directory Name", newDirectoryName, IM_ARRAYSIZE(newDirectoryName));

		if (ImGui::Button("Create") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			// create new file in the currently selected folder, if it does not exist
			Path path = get_project()->get_base_path() / _path / newDirectoryName;

			AssetEngine& assets = AssetEngine::instance();

			// creating new directory
			get_application().create_directory(path);

			memset(newDirectoryName, 0, IM_ARRAYSIZE(newDirectoryName));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			memset(newDirectoryName, 0, IM_ARRAYSIZE(newDirectoryName));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (!inBuiltInDirectory && ImGui::Button("Open Folder"))
	{
		// open the assets folder
		Operations::open(project->get_base_path() / _path);
	}

	ImGui::Separator();

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 itemSize(windowSize.x, 30.0f);
	ImVec2 spacing = ImVec2(0.0f, 1.0f);

	// draw a list of buttons with their paths
	// always draw back
	if (!_path.empty())
	{
		if (ImGui::ButtonAlignedLeft("../", itemSize))
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

		ImGui::Dummy(spacing);
	}

	// draw directories
	for (auto const& path : _directories)
	{
		// if clicked, move to new directory
		if (ImGui::ButtonAlignedLeft(path.string().append("/").c_str(), itemSize))
		{
			// move to new folder
			set_path(_path / path);
			break;
		}

		ImGui::Dummy(spacing);
	}

	Ref<Scene> scene = get_scene();

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
		if (ImGui::ButtonAlignedLeft(name.c_str(), itemSize))
		{
			EditorApplication& app = get_application();

			if (_path.string().starts_with("BuiltIn"))
			{
				app.open_asset(_path / fileData.path);
			}
			else
			{
				app.open_asset(project->get_base_path() / _path / fileData.path);
			}
		}

		// if right clicked, toggle inclusion in the scene
		if (scene && fileData.canIncludeInScene && ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			// inverse selection

			Path projectPath = get_path(fileData.path).lexically_relative(project->get_base_path());

			if (scene->is_registered(projectPath))
			{
				// check for any dependents first
				// if something depends on this asset, do not remove it, log something about needing it
				AssetEngine& assets = AssetEngine::instance();
				EntityRegistry& registry = scene->get_entity_registry();

				// get the ID
				UUID id = assets.read_id(projectPath);
				MINTY_ASSERT(id.valid());

				// get the asset
				Ref<Asset> asset = assets.get_asset(id);
				MINTY_ASSERT(asset != nullptr);

				// get dependents
				std::vector<Ref<Asset>> dependentAssets = assets.get_dependents(asset);
				std::vector<Entity> dependentEntities = registry.get_dependents(asset);

				// register only if none
				if (dependentAssets.empty() && dependentEntities.empty())
				{
					// no dependents
					scene->unregister_asset(projectPath);
				}
				else
				{
					// something else depends on this
					std::vector<String> assetStrings;
					assetStrings.reserve(dependentAssets.size());
					for (Ref<Asset> asset : dependentAssets)
					{
						assetStrings.push_back(asset->get_name());
					}
					std::vector<String> entityStrings;
					entityStrings.reserve(dependentEntities.size());
					for (Entity const entity : dependentEntities)
					{
						entityStrings.push_back(registry.get_name(entity));
					}

					get_application().log_error(
						std::format("Cannot unregister \"{}\". There are dependents: Assets: {}, Entities: {}",
							projectPath.filename().string(),
							assetStrings.empty() ? "(none)" : Text::join(assetStrings),
							entityStrings.empty() ? "(none)" : Text::join(entityStrings)
						));
				}
			}
			else
			{
				scene->register_asset(projectPath);
			}

			// refresh
			get_application().refresh();
		}

		ImGui::Dummy(spacing);
	}

	ImGui::End();
}

void Mintye::AssetsWindow::reset()
{
	// go back to the base folder
	set_path("");
}

void Mintye::AssetsWindow::refresh()
{
	// re-populate the editor files
	set_path(_path);
}

void Mintye::AssetsWindow::set_path(Minty::Path const& path)
{
	// set the new path
	_path = path;

	// clear old data
	_directories.clear();
	_files.clear();

	Project* project = get_project();

	// do nothing if no project loaded
	if (!project) return;

	// if empty path, just do the base asset location options
	if (_path.empty())
	{
		_directories.push_back("Assets");
		_directories.push_back("BuiltIn");

		return;
	}

	Ref<Scene> scene = get_scene();

	static std::unordered_set<AssetType> cannotIncludeToScene
	{
		AssetType::Scene,
		AssetType::Script
	};

	// if BuiltIn, grab from AssetManager
	if (_path.string().starts_with("BuiltIn"))
	{
		// TODO: filter out directories and such

		AssetEngine& assets = AssetEngine::instance();

		Wrapper const& wrapper = assets.get_wrapper();

		for (size_t i = 0; i < wrapper.get_wrap_count(); i++)
		{
			Wrap const& wrap = wrapper.get_wrap(i);

			for (size_t j = 0; j < wrap.get_entry_count(); j++)
			{
				Wrap::Entry const& entry = wrap.get_entry(j);

				// ignore meta
				if (Asset::get_type(entry.path) == AssetType::Meta) continue;

				// for now, add directly
				_files.push_back(FileData
					{
						.path = entry.path,
						.canIncludeInScene = !cannotIncludeToScene.contains(Asset::get_type(entry.path)),
						.includedInScene = scene && scene->is_registered(Path(wrap.get_base_path()) / entry.path),
					});
			}
		}

		return;
	}

	// if Assets, grab from disk
	if (_path.string().starts_with("Assets"))
	{
		// real path
		Path fullPath = project->get_base_path() / _path;

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
					.canIncludeInScene = !cannotIncludeToScene.contains(Asset::get_type(entry.path())),
					.includedInScene = scene && scene->is_registered(get_path(entry.path()).lexically_relative(project->get_base_path())),
					});
			}
		}

		return;
	}
}

Minty::Path Mintye::AssetsWindow::get_path(Minty::Path const& path) const
{
	return get_project()->get_base_path() / _path / path;
}
