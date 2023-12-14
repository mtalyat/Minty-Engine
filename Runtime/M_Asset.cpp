#include "pch.h"
#include "M_Asset.h"

#include "M_File.h"
#include "M_Console.h"

using namespace minty;
using namespace minty::asset;

std::string get_assets_path(std::string const& path)
{
	return (BASE_PATH / path).string();
}

bool minty::asset::exists(std::string const& path)
{
	return file::exists(get_assets_path(path));
}

bool minty::asset::exists_meta(std::string const& path)
{
	return exists(path + ".meta");
}

std::string minty::asset::absolute(std::string const& path)
{
	return file::absolute(get_assets_path(path));
}

Node minty::asset::load_node(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_node(get_assets_path(path));
#endif
}

Node minty::asset::load_meta(std::string const& path)
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

std::vector<char> minty::asset::load_chars(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_all_chars(get_assets_path(path));
#endif
}

std::string minty::asset::load_text(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::string();
#else
	return file::read_all_text(get_assets_path(path));
#endif
}

std::vector<std::string> minty::asset::load_lines(std::string const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::vector<std::string>();
#else
	return file::read_all_lines(get_assets_path(path));
#endif
}