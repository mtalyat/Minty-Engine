#include "pch.h"
#include "M_Assets.h"

#include "M_File.h"
#include "M_Console.h"

using namespace minty;
using namespace minty::assets;

std::string get_assets_path(std::string const& path)
{
	return (BASE_PATH / path).string();
}

bool minty::assets::exists(std::string const& path)
{
	return file::exists(get_assets_path(path));
}

bool minty::assets::exists_meta(std::string const& path)
{
	return exists(path + ".meta");
}

std::string minty::assets::absolute(std::string const& path)
{
	return file::absolute(get_assets_path(path));
}

Node minty::assets::load_node(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_node(get_assets_path(path));
#endif
}

Node minty::assets::load_meta(std::string const& path)
{
	std::string metaPath = path + ".meta";

	if (exists(metaPath))
	{
		return load_node(metaPath);
	}
	else
	{
		// no meta file found
		return Node();
	}
}

std::vector<char> minty::assets::load_chars(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_all_chars(get_assets_path(path));
#endif
}

std::string minty::assets::load_text(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_all_text(get_assets_path(path));
#endif
}

std::vector<std::string> minty::assets::load_lines(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::vector<std::string>();
#else
	return file::read_all_lines(get_assets_path(path));
#endif
}