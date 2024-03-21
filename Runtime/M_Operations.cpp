#include "pch.h"
#include "M_Operations.h"

#include <filesystem>

#ifdef MINTY_WINDOWS
#include <windows.h>

#elif defined(MINTY_APPLE)
#include <cstdlib>

#elif defined(MINTY_LINUX)
#include <cstdlib>

#endif

namespace fs = std::filesystem;
using namespace minty;

// gets the directory of the path
// if the path is a directory, it returns it, otherwise it gets the directory of the item
Path get_directory(Path const& path)
{
	if (path.empty()) return path;

	if (fs::is_directory(path))
	{
		// return this path since it is a directory
		return path;
	}
	else if(path.has_parent_path())
	{
		// return parent of file
		return path.parent_path();
	}
	else
	{
		// no parent, so use empty path
		return Path();
	}
}

void minty::Operations::open_directory(Path const& path)
{
	Path directory = get_directory(path);

	String pathString = directory.string();
#ifdef MINTY_WINDOWS
	std::replace(pathString.begin(), pathString.end(), '/', '\\');
	system(std::format("start explorer \"{}\"", pathString).c_str());
#elif defined(MINTY_APPLE)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system(std::format("xdg-open \"{}\"", pathString).c_str());
#elif defined(MINTY_LINUX)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system(std::format("open \"{}\"", pathString).c_str());
#endif
}

void minty::Operations::open(Path const& path)
{
	String pathString = path.string();
#ifdef MINTY_WINDOWS
	std::replace(pathString.begin(), pathString.end(), '/', '\\');
	system(std::format("start explorer \"{}\"", pathString).c_str());
#elif defined(MINTY_APPLE)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system(std::format("xdg-open \"{}\"", pathString).c_str());
#elif defined(MINTY_LINUX)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system(std::format("open \"{}\"", pathString).c_str());
#endif
}
