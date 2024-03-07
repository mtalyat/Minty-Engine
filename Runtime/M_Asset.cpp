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

Path minty::Asset::get_meta_path(Path const& assetPath)
{
	return Path(assetPath.string() + META_EXTENSION);
}

bool minty::operator==(Asset const& left, Asset const& right)
{
	return left._id == right._id;
}

bool minty::operator!=(Asset const& left, Asset const& right)
{
	return left._id != right._id;
}
