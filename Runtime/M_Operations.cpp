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

#ifdef MINTY_WINDOWS
	system(std::format("start explorer \"{}\"", directory.string()).c_str());
#elif defined(MINTY_APPLE)
	system(std::format("xdg-open \"{}\"", directory.string()).c_str());
#elif defined(MINTY_LINUX)
	system(std::format("open \"{}\"", directory.string()).c_str());
#endif
}

void minty::Operations::open(Path const& path)
{
#ifdef MINTY_WINDOWS
	system(std::format("start explorer \"{}\"", path.string()).c_str());
#elif defined(MINTY_APPLE)
	system(std::format("xdg-open \"{}\"", path.string()).c_str());
#elif defined(MINTY_LINUX)
	system(std::format("open \"{}\"", path.string()).c_str());
#endif
}