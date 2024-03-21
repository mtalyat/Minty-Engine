#pragma once

#include "ME_Minty.h"
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <set>

namespace mintye
{
	/// <summary>
	/// Holds information for a project, which will be ran in the engine.
	/// </summary>
	class Project
	{
	private:
		minty::Info _info;

		// the base minty::Path of the project
		minty::Path _base;

		// files, organized by extension type
		size_t _fileCount;
		std::unordered_map<minty::Path, std::vector<minty::Path>> _files;
	public:
		/// <summary>
		/// Creates a new interface into a project at the given path.
		/// </summary>
		/// <param name="path"></param>
		Project(minty::Path const& path);

		/// <summary>
		/// Gets the info for this Project.
		/// </summary>
		/// <returns></returns>
		minty::Info const& get_info() const;

		/// <summary>
		/// Gets the name of this Project.
		/// </summary>
		/// <returns></returns>
		minty::String const& get_name() const;

		/// <summary>
		/// Gets the base file path for this Project.
		/// </summary>
		/// <returns></returns>
		minty::Path get_base_path() const;

		/// <summary>
		/// Gets the full path to the sub path location from the base path.
		/// </summary>
		/// <param name="subPath"></param>
		/// <returns></returns>
		minty::Path get_sub_path(minty::Path const& subPath) const;

		/// <summary>
		/// Gets the Assets folder path for this project.
		/// </summary>
		/// <returns></returns>
		minty::Path get_assets_path() const;

		/// <summary>
		/// Gets the Build folder path for this project.
		/// </summary>
		/// <returns></returns>
		minty::Path get_build_path() const;

		/// <summary>
		/// Gets the Assembly folder path for this project.
		/// </summary>
		/// <returns></returns>
		minty::Path get_assembly_path() const;

		/// <summary>
		/// Finds all assets within this Project that have the extensions.
		/// </summary>
		/// <param name="extensions">The extensions for the asset type.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<minty::Path> find_assets(std::vector<minty::Path> const& extensions) const;

		/// <summary>
		/// Finds all assets within the Project that have the common file type extensions.
		/// </summary>
		/// <param name="commonFileTypes">The common file types.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<minty::Path> find_assets(minty::AssetType const assetType) const;

		/// <summary>
		/// Finds the first asset that has the given name and file extension.
		/// </summary>
		/// <param name="name">The name and extension of the file asset.</param>
		/// <returns>The path to the file, or "" if none found.</returns>
		minty::Path find_asset(std::string name) const;

		/// <summary>
		/// Finds the first asset that has the given common file type extension.
		/// </summary>
		/// <param name="commonFileTypes"></param>
		/// <returns></returns>
		minty::Path find_asset(minty::AssetType const assetType) const;

		/// <summary>
		/// Finds the first asset that has any of the given file extensions.
		/// </summary>
		/// <param name="extensions"></param>
		/// <returns></returns>
		minty::Path find_asset(std::vector<minty::Path> const& extensions) const;

		/// <summary>
		/// Finds the asset with the given name and given file type.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="commonFileType"></param>
		/// <returns></returns>
		minty::Path find_asset(minty::String const& name, minty::AssetType const assetType) const;

		/// <summary>
		/// Searches the disk for all files and updates their internal states.
		/// </summary>
		void refresh();

		/// <summary>
		/// Returns the number of collected assets.
		/// </summary>
		/// <returns></returns>
		size_t get_asset_count() const;

		/// <summary>
		/// Adds all of the assets in this Project to the given wrap file.
		/// </summary>
		/// <param name="wrap"></param>
		void wrap_assets(minty::Wrap& wrap) const;
	};
}