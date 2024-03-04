#include "pch.h"
#include "M_Asset.h"

#include "M_File.h"
#include "M_Console.h"

using namespace minty;

minty::Asset::Asset()
	: RuntimeObject()
	, _id()
	, _path()
{}

minty::Asset::Asset(UUID const id, Path const& path, Runtime& runtime)
	: RuntimeObject(runtime)
	, _id(id != INVALID_UUID ? id : UUID())
	, _path(path)
{}

minty::Asset::~Asset()
{
}

UUID minty::Asset::get_id() const
{
	return _id;
}

Path const& minty::Asset::get_path() const
{
	return _path;
}

String minty::Asset::get_name() const
{
	return _path.stem().string();
}

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
		MINTY_ASSERT(!path.empty());
		MINTY_ASSERT_FORMAT(Asset::exists(path), "Path does not exist: {}", std::filesystem::absolute(path).string());
		MINTY_ASSERT_FORMAT(!extension || path.extension() == extension, "Path does not have the correct extension. Path: {}, extension: {}", path.string(), extension);
		return 1;
	}

	if (requiresMeta && !Asset::exists_meta(path))
	{
		//Console::error(std::format("Cannot find_animation meta file for asset at path \"{}\".", path.string()));
		// cannot find asset meta file
		MINTY_ASSERT_FORMAT(!requiresMeta || Asset::exists_meta(path), "Missing appropriate meta path for path: {}, missing meta path: {}", std::filesystem::absolute(path).string(), std::filesystem::absolute(path).string() + META_EXTENSION);
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
#if MINTY_RELEASE
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_node(get_assets_path(path));
#endif
}

Node minty::Asset::load_meta(Path const& path)
{
	Path metaPath = Path(std::format("{}{}", path.string(), META_EXTENSION));

	if (Asset::exists(metaPath))
	{
		return load_node(metaPath);
	}
	else
	{
		// no meta file found
		MINTY_WARN_FORMAT("No meta file found at: \"{}\", absolute path: \"{}\"", path.string(), std::filesystem::absolute(path).string());
		return Node();
	}
}

std::vector<char> minty::Asset::load_chars(Path const& path)
{
#if MINTY_RELEASE
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_chars(get_assets_path(path));
#endif
}

String minty::Asset::load_text(Path const& path)
{
#if MINTY_RELEASE
	Console::error("Asset loading not implemented for release builds.");
	return String();
#else
	return File::read_all_text(get_assets_path(path));
#endif
}

std::vector<String> minty::Asset::load_lines(Path const& path)
{
#if MINTY_RELEASE
	Console::error("Asset loading not implemented for release builds.");
	return std::vector<String>();
#else
	return File::read_all_lines(get_assets_path(path));
#endif
}

bool minty::operator==(Asset const& left, Asset const& right)
{
	return left._id == right._id;
}

bool minty::operator!=(Asset const& left, Asset const& right)
{
	return left._id != right._id;
}
