#pragma once

#include <Minty.h>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <set>

namespace mintye
{
	typedef std::filesystem::path filepath;

	/// <summary>
	/// Holds information for a project, which will be ran in the engine.
	/// </summary>
	class Project
	{
	public:
		enum class CommonFileTypes
		{
			Header,			// .h
			Source,			// .cpp .c
			Scene,			// .scene
			Shader,			// .spv
			Texture,		// .png, .jpg, .jpeg, .bmp
			Text,			// .txt
			CSV,			// .csv
		};

	private:
		// the base filepath of the project
		filepath _base;

		std::unordered_map<std::string, std::set<filepath>> _files;
	public:
		/// <summary>
		/// Creates a new interface into a project at the given path.
		/// </summary>
		/// <param name="path"></param>
		Project(std::string const& path);

		/// <summary>
		/// Gets the base file path for this project.
		/// </summary>
		/// <returns></returns>
		filepath const get_base_path() const;

		/// <summary>
		/// Gets the Assets folder path for this project.
		/// </summary>
		/// <returns></returns>
		filepath const get_assets_path() const;

		/// <summary>
		/// Gets the Build folder path for this project.
		/// </summary>
		/// <returns></returns>
		filepath const get_build_path() const;

		/// <summary>
		/// Finds all assets within this Project that have the extensions.
		/// </summary>
		/// <param name="extensions">The extensions for the asset type.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<filepath> find_assets(std::set<std::string> const& extensions) const;

		/// <summary>
		/// Finds all assets within the Project that have the common file type extensions.
		/// </summary>
		/// <param name="commonFileTypes">The common file types.</param>
		/// <returns>The filepaths to the assets with one of the given extensions.</returns>
		std::set<filepath> find_assets(CommonFileTypes const commonFileTypes) const;

		/// <summary>
		/// Finds the first asset that has the given name and file extension.
		/// </summary>
		/// <param name="name">The name and extension of the file asset.</param>
		/// <returns>The path to the file, or "" if none found.</returns>
		filepath find_asset(std::string name) const;

		/// <summary>
		/// Searches the disk for all files and updates their internal states. Use this to "refresh" the project.
		/// </summary>
		void collect_assets();
	};
}