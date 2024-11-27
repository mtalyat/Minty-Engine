#include "pch.h"
#include "ME_Project.h"

#include "ME_Constants.h"
#include <vector>

using namespace Minty;
using namespace Mintye;

Mintye::Project::Project(Minty::Path const& path)
	: _base(std::filesystem::absolute(path))
	, _info({
		.name = path.stem().string(),
		.version = MINTY_MAKE_VERSION(1, 0, 0)
		})
	, _fileCount()
	, _files()
{}

Minty::Path Mintye::Project::get_sub_path(Minty::Path const& subPath) const
{
	return (_base / subPath);
}

Path Project::get_assets_path() const
{
	return (_base / ASSETS_DIRECTORY_NAME);
}

Path Project::get_build_path() const
{
	return (_base / BUILD_DIRECTORY_NAME);
}

Minty::Path Mintye::Project::get_assembly_path() const
{
	return (_base / ASSEMBLY_DIRECTORY_NAME);
}

std::set<Path> Mintye::Project::find_assets(std::vector<Minty::Path> const& extensions) const
{
	// output files
	std::set<Path> result;

	// find all files with headers and add to result
	for (Path const& extension : extensions)
	{
		auto found = _files.find(extension);
		if (found != _files.end())
		{
			// add all to result
			result.insert(found->second.begin(), found->second.end());
		}
	}

	return result;
}

std::set<Path> Mintye::Project::find_assets(AssetType const assetType) const
{
	return find_assets(Asset::get_extensions(assetType));
}

Path Mintye::Project::find_asset(std::string name) const
{
	// get the path
	Path path(name);

	// get the extension
	std::string extension = path.extension().string();

	// get the name without the extension
	Path stem = path.stem();

	// check files with extension
	auto found = _files.find(extension);

	if (found != _files.end())
	{
		for (Path const& path : found->second)
		{
			if (path.stem() == stem)
			{
				// match found
				return path;
			}
		}
	}

	// no match found
	return Path();
}

Minty::Path Mintye::Project::find_asset(AssetType const assetType) const
{
	return find_asset(Asset::get_extensions(assetType));
}

Minty::Path Mintye::Project::find_asset(std::vector<Minty::Path> const& extensions) const
{
	// check each extension, if it exists
	for (auto const& extension : extensions)
	{
		auto found = _files.find(extension);

		if (found != _files.end())
		{
			if (found->second.size())
			{
				return found->second.front();
			}
		}
	}

	// none found
	return Path();
}

Minty::Path Mintye::Project::find_asset(Minty::String const& name, AssetType const assetType) const
{
	for (auto const& extension : Asset::get_extensions(assetType))
	{
		Path path = find_asset(std::format("{}{}", name, extension.string()));

		// asset found with name and type, return path to it
		if (!path.empty())
		{
			return path;
		}
	}

	// none found
	return Path();
}

void Project::refresh()
{
	if (!std::filesystem::exists(_base.string()))
	{
		MINTY_ERROR_FORMAT("Project missing project directory at path: {}", _base.string());
		return;
	}

	Path assetsPath = get_assets_path();

	if (!std::filesystem::exists(assetsPath))
	{
		MINTY_ERROR_FORMAT("Project missing assets directory at path: {}", _base.string());
		return;
	}

	// clear old data
	_fileCount = 0;
	_files.clear();

	// list of directories to collect from
	std::vector<Path> directoriesToCollect;

	// add base directory to get started
	directoriesToCollect.push_back(assetsPath);

	Path directory;

	// keep collecting while paths to collect has something in it
	while (!directoriesToCollect.empty())
	{
		// get last element
		directory = directoriesToCollect.back();

		// remove it
		directoriesToCollect.pop_back();

		// search the path
		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			// get the path, relative to the assets folder
			Path path = entry.path();

			if (entry.is_directory())
			{
				// if another directory, we want to search it later
				directoriesToCollect.push_back(path);
			}
			else
			{
				// if a file, check the file type and add where necessary
				Path relativePath = path.lexically_relative(_base);
				Path extension = path.extension();

				// if header exists in files, add to that list
				// otherwise add to new list
				auto found = _files.find(extension);
				if (found == _files.end())
				{
					// new list
					_files.emplace(extension, std::vector<Path>());
					_files.at(extension).push_back(relativePath);
				}
				else
				{
					// existing list
					found->second.push_back(relativePath);
				}

				// if the file needs a meta, create one with a new ID quick
				if (extension != EXTENSION_META && !std::filesystem::exists(Asset::get_meta_path(path)))
				{
					// create a new meta with a random ID, the rest can be populated later
					// (ID is the most important for asset loading)
					UUID id = UUID::create();
					String text = std::format(": {}", to_string(id));
					File::write_all_text(Asset::get_meta_path(path), text);
				}

				_fileCount++;
			}
		}
	}
}

Size Mintye::Project::get_asset_count() const
{
	return _fileCount;
}

void Mintye::Project::wrap_assets(Wrap& wrap) const
{
	for (auto const& [extension, paths] : _files)
	{
		for (auto const& path : paths)
		{
			wrap.emplace(path, path);
		}
	}
}
