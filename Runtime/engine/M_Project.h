#pragma once

#include <string>
#include <filesystem>
#include <set>

namespace minty
{
	typedef std::filesystem::path filepath;

	/// <summary>
	/// Holds information for a project, which will be ran in the engine.
	/// </summary>
	class Project
	{
	private:
		// the base filepath of the project
		filepath const _base;

		// all of the header (.h) files in the Assets folder
		std::set<filepath> _headers;
		std::set<std::string> const _headerExtensions = { ".h" };

		// all of the source (.cpp) files in the Assets folder
		std::set<filepath> _sources;
		std::set<std::string> const _sourceExtensions = { ".cpp" };

		// all of the scene (.scene) files in the Assets folder
		std::set<filepath> _scenes;
		std::set<std::string> const _sceneExtensions = { ".scene" };

		// everything else
		std::set<filepath> _others;

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
		filepath const getBasePath() const;

		/// <summary>
		/// Gets the Assets folder path for this project.
		/// </summary>
		/// <returns></returns>
		filepath const getAssetsPath() const;

		/// <summary>
		/// Gets the Build folder path for this project.
		/// </summary>
		/// <returns></returns>
		filepath const getBuildPath() const;

		/// <summary>
		/// Returns a set of all header files in this Project.
		/// </summary>
		/// <returns></returns>
		std::set<filepath> const& getAssetsHeaderPaths() const;

		/// <summary>
		/// Returns a set of all source files in this Project.
		/// </summary>
		/// <returns></returns>
		std::set<filepath> const& getAssetsSourcePaths() const;

		/// <summary>
		/// Returns a set of all scene files in this Project.
		/// </summary>
		/// <returns></returns>
		std::set<filepath> const& getAssetsScenePaths() const;
	private:
		// searches through the project assets folder and finds all files of certain types
		void collectAssets();
	};
}