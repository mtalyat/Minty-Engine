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

int minty::Asset::check(Path const& path, char const* const extension, bool const requiresMeta)
{
	// can load if assets exists, and if no meta is required, or if a meta is required, it exists
	// also needs to have the correct extension, if one was provided
	if (path.empty() || !Asset::exists(path) || (extension && path.extension() != extension))
	{
		//Console::error(std::format("Cannot find_animation asset at path \"{}\".", path.string()));
		// cannot find asset itself
		MINTY_ASSERT(!path.empty(), "Asset::check(): Path is empty.");
		MINTY_ASSERT(Asset::exists(path), std::format("Asset::check(): Path does not exist: {}", path.string()));
		MINTY_ASSERT(!extension || path.extension() == extension, std::format("Asset::check(): Path does not have the correct extension. Path: {}, extension: {}", path.string(), extension));
		return 1;
	}

	if (requiresMeta && !Asset::exists_meta(path))
	{
		//Console::error(std::format("Cannot find_animation meta file for asset at path \"{}\".", path.string()));
		// cannot find asset meta file
		MINTY_ASSERT(!requiresMeta || Asset::exists_meta(path), std::format("Asset::check(): Missing appropriate meta path for path: {}, missing meta path: {}", path.string(), path.string() + META_EXTENSION));
		return 2;
	}

	// found both
	return 0;
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
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_node(get_assets_path(path));
#endif
}

Node minty::Asset::load_meta(Path const& path)
{
	Path metaPath = Path(path).replace_extension(META_EXTENSION);

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
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_chars(get_assets_path(path));
#endif
}

String minty::Asset::load_text(Path const& path)
{
#if N_DEBUG
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_text(get_assets_path(path));
#endif
}

std::vector<String> minty::Asset::load_lines(Path const& path)
{
#if N_DEBUG
	Console::error("Asset loading not implemented for release builds.");
	return std::vector<String>();
#else
	return File::read_all_lines(get_assets_path(path));
#endif
}