#include "pch.h"
#include "ME_Project.h"

#include "M_Console.h"
#include "M_File.h"
#include <vector>

using namespace minty;
using namespace mintye;

Project::Project(std::string const& path)
	: _base(std::filesystem::absolute(path))
	, _files()
{
	collect_assets();
}

filepath const Project::get_base_path() const
{
	return _base;
}

filepath const Project::get_assets_path() const
{
	return (_base / "Assets");
}

filepath const Project::get_build_path() const
{
	return (_base / "Build");
}

std::set<filepath> mintye::Project::find_assets(std::set<std::string> const& extensions) const
{
	// output files
	std::set<filepath> result;

	// find all files with headers and add to result
	for (std::string const& extension : extensions)
	{
		auto const& found = _files.find(extension);
		if (found != _files.end())
		{
			// add all to result
			result.insert(found->second.begin(), found->second.end());
		}
	}

	return result;
}

std::set<filepath> mintye::Project::find_assets(CommonFileTypes const commonFileTypes) const
{
	// switch based on common file types
	switch (commonFileTypes)
	{
	case CommonFileTypes::Header: return find_assets({ ".h" });
	case CommonFileTypes::Source: return find_assets({ ".cpp", ".c" });
	case CommonFileTypes::Scene: return find_assets({ ".scene" });
	case CommonFileTypes::Shader: return find_assets({ ".spv" });
	case CommonFileTypes::Texture: return find_assets({ ".png", ".jpg", ".jpeg", ".bmp" });
	case CommonFileTypes::Text: return find_assets({ ".txt" });
	case CommonFileTypes::CSV: return find_assets({ ".csv" });
	default: return {}; // not a valid common file type
	}
}

filepath mintye::Project::find_asset(std::string name) const
{
	// get the path
	filepath path(name);

	// get the extension
	std::string extension = path.extension().string();
	
	// get the name without the extension
	filepath stem = path.stem();

	// check files with extension
	auto const& found = _files.find(extension);
	
	if (found != _files.end())
	{
		for (filepath const& path : found->second)
		{
			if (path.stem() == stem)
			{
				// match found
				return path;
			}
		}
	}

	// no match found
	return filepath();
}

void Project::collect_assets()
{
	if (!file::exists(_base.string()))
	{
		return;
	}

	// list of directories to collect from
	std::vector<filepath> directoriesToCollect;

	// add base directory to get started
	directoriesToCollect.push_back(get_assets_path());

	filepath directory;

	size_t count = 0;

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

				// if header exists in files, add to that list
				// otherwise add to new list
				auto const& found = _files.find(extension);
				if (found == _files.end())
				{
					// new list
					_files.emplace(extension, std::set<filepath>());
					_files.at(extension).emplace(path);
				}
				else
				{
					// existing list
					found->second.emplace(path);
				}

				count++;
			}
		}
	}

	//console::log(std::format("Found {} assets:", count));
	//for (auto const& pair : _files)
	//{
	//	console::log(std::format("\t{}:", pair.first));
	//	for (auto const& path : pair.second)
	//	{
	//		console::log(std::format("\t\t{}", path.string()));
	//	}
	//}
}