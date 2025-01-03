#pragma once

#include "Base.h"
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <set>

namespace Mintye
{
	/// <summary>
	/// Holds information for a project, which will be ran in the engine.
	/// </summary>
	class Project
	{
	private:
		// the base Minty::Path of the project
		Minty::Path m_base;

		// the application info for the project
		Minty::ApplicationInfo m_info;

		// files, organized by extension type
		Minty::Size m_fileCount;
		std::unordered_map<Minty::Path, std::vector<Minty::Path>> m_files;
	public:
		/// <summary>
		/// Creates a new interface into a project at the given path.
		/// </summary>
		/// <param name="path"></param>
		Project(Minty::Path const& path);

		/// <summary>
		/// Gets the ApplicationInfo for this project.
		/// </summary>
		/// <returns></returns>
		Minty::ApplicationInfo& get_info() { return m_info; }

		/// <summary>
		/// Gets the ApplicationInfo for this project.
		/// </summary>
		/// <returns></returns>
		Minty::ApplicationInfo const& get_info() const { return m_info; }

		/// <summary>
		/// Gets the name of this Project.
		/// </summary>
		/// <returns></returns>
		Minty::String const& get_name() const { return m_info.name; }

		/// <summary>
		/// Gets the base file path for this Project.
		/// </summary>
		/// <returns></returns>
		Minty::Path get_base_path() const { return m_base; }

		/// <summary>
		/// Gets the full path to the sub path location from the base path.
		/// </summary>
		/// <param name="subPath"></param>
		/// <returns></returns>
		Minty::Path get_sub_path(Minty::Path const& subPath) const;

		/// <summary>
		/// Gets the Assets folder path for this project.
		/// </summary>
		/// <returns></returns>
		Minty::Path get_assets_path() const;

		/// <summary>
		/// Gets the Build folder path for this project.
		/// </summary>
		/// <returns></returns>
		Minty::Path get_build_path() const;

		/// <summary>
		/// Gets the Assembly folder path for this project.
		/// </summary>
		/// <returns></returns>
		Minty::Path get_assembly_path() const;

		/// <summary>
		/// Finds all assets within this Project that have the extensions.
		/// </summary>
		/// <param name="extensions">The extensions for the asset type.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<Minty::Path> find_assets(std::vector<Minty::Path> const& extensions) const;

		/// <summary>
		/// Finds all assets within the Project that have the common file type extensions.
		/// </summary>
		/// <param name="commonFileTypes">The common file types.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<Minty::Path> find_assets(Minty::AssetType const assetType) const;

		/// <summary>
		/// Finds the first asset that has the given name and file extension.
		/// </summary>
		/// <param name="name">The name and extension of the file asset.</param>
		/// <returns>The path to the file, or "" if none found.</returns>
		Minty::Path find_asset(std::string name) const;

		/// <summary>
		/// Finds the first asset that has the given common file type extension.
		/// </summary>
		/// <param name="commonFileTypes"></param>
		/// <returns></returns>
		Minty::Path find_asset(Minty::AssetType const assetType) const;

		/// <summary>
		/// Finds the first asset that has any of the given file extensions.
		/// </summary>
		/// <param name="extensions"></param>
		/// <returns></returns>
		Minty::Path find_asset(std::vector<Minty::Path> const& extensions) const;

		/// <summary>
		/// Finds the asset with the given name and given file type.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="commonFileType"></param>
		/// <returns></returns>
		Minty::Path find_asset(Minty::String const& name, Minty::AssetType const assetType) const;

		/// <summary>
		/// Searches the disk for all files and updates their internal states.
		/// </summary>
		void refresh();

		/// <summary>
		/// Returns the number of collected assets.
		/// </summary>
		/// <returns></returns>
		Minty::Size get_asset_count() const;

		/// <summary>
		/// Adds all of the assets in this Project to the given wrap file.
		/// </summary>
		/// <param name="wrap"></param>
		void wrap_assets(Minty::Wrap& wrap) const;
	};
}