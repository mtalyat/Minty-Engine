#include "pch.h"
#include "M_Project.h"

#include "M_Console.h"
#include <vector>

using namespace minty;

Project::Project(std::string const& path)
	: _base(std::filesystem::absolute(path))
{
	collectAssets();
}

filepath const Project::getBasePath() const
{
	return _base;
}

filepath const Project::getAssetsPath() const
{
	return (_base / "Assets");
}

filepath const Project::getBuildPath() const
{
	return (_base / "Build");
}

std::set<filepath> const& minty::Project::getAssetsHeaderPaths() const
{
	return _headers;
}

std::set<filepath> const& minty::Project::getAssetsSourcePaths() const
{
	return _sources;
}

std::set<filepath> const& minty::Project::getAssetsScenePaths() const
{
	return _scenes;
}

void minty::Project::collectAssets()
{
	// list of directories to collect from
	std::vector<filepath> directoriesToCollect;

	// add base directory to get started
	directoriesToCollect.push_back(getAssetsPath());

	filepath directory;

	// keep collecting while paths to collect has something in it
	while (directoriesToCollect.size())
	{
		// get last element
		directory = directoriesToCollect.back();

		// remove it
		directoriesToCollect.pop_back();

		// search the path
		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			filepath path = entry.path();

			if (entry.is_directory())
			{
				// if another directory, we want to search it later
				directoriesToCollect.push_back(path);
			}
			else
			{
				// if a file, check the file type and add where necessary
				std::string extension = path.extension().string();
				
				// add to appropriate collection of file names
				if (_headerExtensions.contains(extension))
				{
					_headers.emplace(path);
				}
				else if (_sourceExtensions.contains(extension))
				{
					_sources.emplace(path);
				}
				else if (_sceneExtensions.contains(extension))
				{
					_scenes.emplace(path);
				}
				else
				{
					// must be something else
					_others.emplace(path);
				}
			}
		}
	}
}