#include "pch.h"
#include "M_Operations.h"

#include <filesystem>

// for getenv
#include <cstdlib>

#ifdef MINTY_WINDOWS
#include <windows.h>

//#elif defined(MINTY_APPLE)
//#include <cstdlib>
//
//#elif defined(MINTY_LINUX)
//#include <cstdlib>

#endif

namespace fs = std::filesystem;
using namespace Minty;

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

void Minty::Operations::open_directory(Path const& path)
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

void Minty::Operations::open(Path const& path)
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

void Minty::Operations::copy(Path const& from, Path const& to)
{
	Path dest;
	if (to.has_extension())
	{
		dest = to;
	}
	else
	{
		dest = to / from.filename();
	}

	try {
		std::filesystem::copy(from, dest, std::filesystem::copy_options::overwrite_existing);
	}
	catch (std::filesystem::filesystem_error& e)
	{
		MINTY_ERROR_FORMAT("Failed to copy \"{}\" to \"{}\": \"{}\"", from.generic_string(), dest.generic_string(), e.what());
	}
}

void Minty::Operations::copy_all(Path const& from, Path const& extension, Path const& to)
{
	for (auto entry : std::filesystem::directory_iterator(from))
	{
		if (extension.empty() || (entry.path().has_extension() && entry.path().extension() == extension))
		{
			Operations::copy(entry.path(), to);
		}
	}
}

String Minty::Operations::get_environment_variable(String const& name)
{
	char* buffer = nullptr;
	size_t bufferSize = 0;

	errno_t err = _dupenv_s(&buffer, &bufferSize, name.c_str());

	if (err)
	{
		MINTY_ERROR_FORMAT("Error retrieving \"{}\" environment variable.", name);

		return "";
	}
	else if (buffer == nullptr)
	{
		MINTY_ERROR_FORMAT("\"{}\" environment variable has not been set.", name);

		return "";
	}
	else {
		// variable set
		String result(buffer);

		free(buffer);

		return result;
	}	
}

Path Minty::Operations::get_minty_path()
{
	return get_environment_variable("MINTY_PATH");
}
