#include "pch.h"
#include "ME_Project.h"

#include "ME_Constants.h"
#include "M_Console.h"
#include "M_File.h"
#include <vector>

using namespace minty;
using namespace mintye;

Project::Project(minty::Path const& path)
	: _info(path.stem().string(), 0, 0, 0)
	, _base(std::filesystem::absolute(path))
	, _files()
{}

minty::Info const& mintye::Project::get_info() const
{
	return _info;
}

minty::String const& mintye::Project::get_name() const
{
	return _info.get_application_name();
}

Path Project::get_base_path() const
{
	return _base;
}

minty::Path mintye::Project::get_sub_path(minty::Path const& subPath) const
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

std::set<Path> mintye::Project::find_assets(std::unordered_set<minty::Path> const& extensions) const
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

std::set<Path> mintye::Project::find_assets(CommonFileType const commonFileTypes) const
{
	return find_assets(get_extensions(commonFileTypes));
}

Path mintye::Project::find_asset(std::string name) const
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

minty::Path mintye::Project::find_asset(CommonFileType const commonFileType) const
{
	return find_asset(get_extensions(commonFileType));
}

minty::Path mintye::Project::find_asset(std::unordered_set<minty::Path> const& extensions) const
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

minty::Path mintye::Project::find_asset(minty::String const& name, CommonFileType const commonFileType) const
{
	for (auto const& extension : get_extensions(commonFileType))
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

void Project::collect_assets()
{
	if (!std::filesystem::exists(_base.string()))
	{
		Console::error(std::format("Project missing project directory at path: {}", _base.string()));
		return;
	}

	Path assetsPath = get_assets_path();

	if (!std::filesystem::exists(assetsPath))
	{
		Console::error(std::format("Project missing assets directory at path: {}", _base.string()));
		return;
	}

	// clear old data
	_files.clear();

	// list of directories to collect from
	std::vector<Path> directoriesToCollect;

	// add base directory to get started
	directoriesToCollect.push_back(assetsPath);

	Path directory;

	size_t count = 0;

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
				Path relativePath = path.lexically_relative(assetsPath);
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

				count++;
			}
		}
	}
}

std::unordered_set<minty::Path> mintye::Project::get_extensions(CommonFileType const commonFileType) const
{
	// switch based on common file types
	switch (commonFileType)
	{
	case CommonFileType::Header: return { ".h" };
	case CommonFileType::Source: return { ".cpp", ".c" };
	case CommonFileType::Scene: return { minty::SCENE_EXTENSION };
	case CommonFileType::Text: return { ".txt" };
	case CommonFileType::CSV: return { ".csv" };
	case CommonFileType::Texture: return { ".png", ".jpg", ".jpeg", ".bmp" };
	case CommonFileType::Sprite: return { minty::SPRITE_EXTENSION };
	case CommonFileType::Material: return { minty::MATERIAL_EXTENSION };
	case CommonFileType::MaterialTemplate: return { minty::MATERIAL_TEMPLATE_EXTENSION };
	case CommonFileType::ShaderPass: return { minty::SHADER_PASS_EXTENSION };
	case CommonFileType::Shader: return { minty::SHADER_EXTENSION };
	case CommonFileType::ShaderModule: return { ".spv" };
	case CommonFileType::Audio: return { ".wav", ".mp3" };
	case CommonFileType::Animator: return { minty::ANIMATOR_EXTENSION };
	case CommonFileType::Animation: return { minty::ANIMATION_EXTENSION };
	case CommonFileType::Model: return { ".obj" };
	case CommonFileType::Wrap: return { minty::WRAP_EXTENSION };
	default: return {}; // not a valid common file type
	}
}
