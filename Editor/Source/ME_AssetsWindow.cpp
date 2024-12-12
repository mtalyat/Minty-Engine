#include "ME_AssetsWindow.h"

#include "ME_EditorApplication.h"
#include "ME_Project.h"
#include "ME_ConsoleWindow.h"
#include "ME_Constants.h"
#include <filesystem>

using namespace Minty;
using namespace Mintye;
namespace fs = std::filesystem;

Mintye::AssetsWindow::AssetsWindow(EditorApplication& application)
	: EditorWindow(application)
	, m_path()
	, m_directories()
	, m_files()
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

	GUI::set_next_window_size(Float2(400, 400), GuiCondition::FirstUseEver);

	// do not draw if no project loaded
	if (!GUI::begin("Assets") || !project)
	{
		GUI::end();
		return;
	}

	if (GUI::button("Refresh"))
	{
		get_application().refresh();
	}

	GUI::same_line();

	bool inBuiltInDirectory = m_path.string().starts_with("BuiltIn");

	static bool popupOpen = false;

	if (!inBuiltInDirectory && GUI::button("New File"))
	{
		// new file popup
		GUI::open_popup("Create New Asset File");
		popupOpen = true;
	}

	if (GUI::begin_popup_modal("Create New Asset File"))
	{
		static char newAssetName[255];

		GUI::text("Create New Asset File");

		if (popupOpen)
		{
			GUI::set_keyboard_focus_here();
			popupOpen = false;
		}

		GUI::input_text("Asset Name", newAssetName, sizeof(newAssetName));

		if (GUI::button("Create") || GUI::is_key_pressed(Key::Enter))
		{
			// create new file in the currently selected folder, if it does not exist
			Path path = get_project()->get_base_path() / m_path / newAssetName;

			if (!AssetManager::exists(path))
			{
				// creating new asset
				get_application().create_asset(path);
			}

			memset(newAssetName, 0, sizeof(newAssetName));
			GUI::close_current_popup();
		}

		GUI::same_line();

		if (GUI::button("Cancel") || GUI::is_key_pressed(Key::Escape))
		{
			memset(newAssetName, 0, sizeof(newAssetName));
			GUI::close_current_popup();
		}

		GUI::end_popup();
	}

	GUI::same_line();

	if (!inBuiltInDirectory && GUI::button("New Directory"))
	{
		// new file popup
		GUI::open_popup("Create New Asset Directory");
		popupOpen = true;
	}

	if (GUI::begin_popup_modal("Create New Asset Directory"))
	{
		static char newDirectoryName[255];

		GUI::text("Create New Asset Directory");

		if (popupOpen)
		{
			GUI::set_keyboard_focus_here();
			popupOpen = false;
		}

		GUI::input_text("Directory Name", newDirectoryName, sizeof(newDirectoryName));

		if (GUI::button("Create") || GUI::is_key_pressed(Key::Enter))
		{
			// create new file in the currently selected folder, if it does not exist
			Path path = get_project()->get_base_path() / m_path / newDirectoryName;

			// creating new directory
			get_application().create_directory(path);

			memset(newDirectoryName, 0, sizeof(newDirectoryName));
			GUI::close_current_popup();
		}

		GUI::same_line();

		if (GUI::button("Cancel") || GUI::is_key_pressed(Key::Escape))
		{
			memset(newDirectoryName, 0, sizeof(newDirectoryName));
			GUI::close_current_popup();
		}

		GUI::end_popup();
	}

	GUI::same_line();

	if (!inBuiltInDirectory && GUI::button("Open Folder"))
	{
		// open the assets folder
		Operation::open(project->get_base_path() / m_path);
	}

	GUI::separator();

	Float2 windowSize = GUI::get_window_size();
	Float2 itemSize(windowSize.x, 30.0f);
	Float2 spacing = Float2(0.0f, 1.0f);

	// draw a list of buttons with their paths
	// always draw back
	if (!m_path.empty())
	{
		if (GUI::button_left("../", itemSize))
		{
			// go back
			if (!m_path.has_parent_path())
			{
				// go back to Assets
				set_path("");
			}
			else
			{
				// go to previous folder
				set_path(m_path.parent_path());
			}
		}
		GUI::dummy(spacing);
	}

	// draw directories
	for (auto const& path : m_directories)
	{
		// if clicked, move to new directory
		if (GUI::button_left(path.string().append("/").c_str(), itemSize))
		{
			// move to new folder
			set_path(m_path / path);
			break;
		}

		GUI::dummy(spacing);
	}

	Ref<Scene> scene = get_scene();

	// draw files
	for (auto const& fileData : m_files)
	{
		String name = fileData.path.string();

		if (scene.get() && fileData.canIncludeInScene)
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
		if (GUI::button_left(name.c_str(), itemSize))
		{
			EditorApplication& app = get_application();

			if (m_path.string().starts_with(BUILT_IN_DIRECTORY_NAME))
			{
				app.open_asset(m_path / fileData.path);
			}
			else
			{
				app.open_asset(project->get_base_path() / m_path / fileData.path);
			}
		}

		// if right clicked, toggle inclusion in the scene
		if (scene.get() && fileData.canIncludeInScene && GUI::is_item_clicked(MouseButton::Right))
		{
			// inverse selection

			Path projectPath = get_path(fileData.path).lexically_relative(project->get_base_path());

			if (scene->is_registered(projectPath))
			{
				// check for any dependents first
				// if something depends on this asset, do not remove it, log something about needing it
				EntityRegistry& registry = scene->get_entity_registry();

				// get the ID
				UUID id = AssetManager::read_id(projectPath);
				MINTY_ASSERT(id.valid());

				// get the asset
				Ref<Asset> asset = AssetManager::get_asset(id);

				// ignore if not loaded
				if (asset == nullptr)
				{
					// no dependencies, and not loaded, so just unregister it
					scene->unregister_asset(projectPath);
				} else
				{
					// get dependents
					std::vector<Ref<Asset>> dependentAssets = AssetManager::get_dependents(asset);
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
							assetStrings.push_back(AssetManager::get_name(asset->id()));
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
								assetStrings.empty() ? "(none)" : join(assetStrings),
								entityStrings.empty() ? "(none)" : join(entityStrings)
							));
					}
				}
			}
			else
			{
				scene->register_asset(projectPath);
			}

			// refresh
			get_application().refresh();
		}

		GUI::dummy(spacing);
	}

	GUI::end();
}

void Mintye::AssetsWindow::reset()
{
	// go back to the base folder
	set_path("");
}

void Mintye::AssetsWindow::refresh()
{
	// re-populate the editor files
	set_path(m_path);
}

void Mintye::AssetsWindow::set_path(Minty::Path const& path)
{
	// set the new path
	m_path = path;

	// clear old data
	m_directories.clear();
	m_files.clear();

	Project* project = get_project();

	// do nothing if no project loaded
	if (!project) return;

	// if empty path, just do the base asset location options
	if (m_path.empty())
	{
		m_directories.push_back("Assets");
		m_directories.push_back(BUILT_IN_DIRECTORY_NAME);

		return;
	}

	Ref<Scene> scene = get_scene();

	static std::unordered_set<AssetType> cannotIncludeToScene
	{
		AssetType::Scene,
		AssetType::Script
	};

	// if BuiltIn, grab from AssetManager
	if (m_path.string().starts_with(BUILT_IN_DIRECTORY_NAME))
	{
		// TODO: filter out directories and such

		Wrapper const& wrapper = AssetManager::get_wrapper();

		for (Size i = 0; i < wrapper.get_wrap_count(); i++)
		{
			Wrap const& wrap = wrapper.get_wrap(i);

			for (Size j = 0; j < wrap.get_entry_count(); j++)
			{
				Wrap::Entry const& entry = wrap.get_entry(j);

				// ignore meta
				if (Asset::get_type_from_path(entry.path) == AssetType::Meta) continue;

				// for now, add directly
				m_files.push_back(FileData
					{
						.path = entry.path,
						.canIncludeInScene = !cannotIncludeToScene.contains(Asset::get_type_from_path(entry.path)),
						.includedInScene = scene.get() && scene->is_registered(Path(wrap.get_base_path()) / entry.path),
					});
			}
		}

		return;
	}

	// if Assets, grab from disk
	if (m_path.string().starts_with("Assets"))
	{
		// real path
		Path fullPath = project->get_base_path() / m_path;

		// update directories and paths
		for (auto const& entry : fs::directory_iterator(fullPath))
		{
			// ignore hidden directories (such as .vscode)
			if (fs::is_directory(entry.status()) && !fullPath.stem().string().starts_with("."))
			{
				m_directories.push_back(entry.path().stem());
			}
			// add all regular files that aren't meta
			else if (fs::is_regular_file(entry.status()) && Asset::get_type_from_path(entry.path()) != AssetType::Meta)
			{
				m_files.push_back(FileData{
					.path = entry.path().filename(),
					.canIncludeInScene = !cannotIncludeToScene.contains(Asset::get_type_from_path(entry.path())),
					.includedInScene = scene.get() && scene->is_registered(get_path(entry.path()).lexically_relative(project->get_base_path())),
					});
			}
		}

		return;
	}
}

Minty::Path Mintye::AssetsWindow::get_path(Minty::Path const& path) const
{
	return get_project()->get_base_path() / m_path / path;
}
