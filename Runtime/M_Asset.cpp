#include "pch.h"
#include "M_Asset.h"

#include "M_File.h"
#include "M_Console.h"

using namespace minty;
using namespace minty::Asset;

Path get_assets_path(Path const& path)
{
	return BASE_PATH / path;
}

bool minty::Asset::exists(Path const& path)
{
	return std::filesystem::exists(get_assets_path(path));
}

bool minty::Asset::exists_meta(Path const& path)
{
	return std::filesystem::exists(Path(path).replace_extension(".meta"));
}

Path minty::Asset::absolute(Path const& path)
{
	return std::filesystem::absolute(get_assets_path(path));
}

Node minty::Asset::load_node(Path const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_node(get_assets_path(path));
#endif
}

Node minty::Asset::load_meta(Path const& path)
{
	Path metaPath = Path(path).replace_extension(".meta");

	if (std::filesystem::exists(metaPath))
	{
		return load_node(metaPath);
	}
	else
	{
		// no meta file found
		return Node();
	}
}

std::vector<char> minty::Asset::load_chars(Path const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_chars(get_assets_path(path));
#endif
}

String minty::Asset::load_text(Path const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_text(get_assets_path(path));
#endif
}

std::vector<String> minty::Asset::load_lines(Path const& path)
{
#if N_DEBUG
	console::error("Asset loading not implemented for release builds.");
	return std::vector<String>();
#else
	return File::read_all_lines(get_assets_path(path));
#endif
}