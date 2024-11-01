#include "pch.h"
#include "Operations.h"

#include <filesystem>

// for getenv, system
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
	else if (path.has_parent_path())
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

int Minty::Operations::system_command(String const& command)
{
	return system(command.c_str());
}

void Minty::Operations::open_directory(Path const& path)
{
	Path directory = get_directory(path);

	String pathString = directory.string();
#ifdef MINTY_WINDOWS
	std::replace(pathString.begin(), pathString.end(), '/', '\\');
	system_command(std::format("start explorer \"{}\"", pathString).c_str());
#elif defined(MINTY_APPLE)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system_command(std::format("xdg-open \"{}\"", pathString).c_str());
#elif defined(MINTY_LINUX)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system_command(std::format("open \"{}\"", pathString).c_str());
#endif
}

void Minty::Operations::open(Path const& path)
{
	String pathString = path.string();
#ifdef MINTY_WINDOWS
	std::replace(pathString.begin(), pathString.end(), '/', '\\');
	system_command(std::format("start explorer \"{}\"", pathString).c_str());
#elif defined(MINTY_APPLE)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system_command(std::format("xdg-open \"{}\"", pathString).c_str());
#elif defined(MINTY_LINUX)
	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	system_command(std::format("open \"{}\"", pathString).c_str());
#endif
}

Bool Minty::Operations::copy(Path const& from, Path const& to)
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
		return false;
	}

	return true;
}

Bool Minty::Operations::copy_files(Path const& from, Path const& extension, Path const& to)
{
	for (auto const& entry : std::filesystem::directory_iterator(from))
	{
		if (extension.empty() || (entry.path().has_extension() && entry.path().extension() == extension))
		{
			if (!Operations::copy(entry.path(), to))
			{
				return false;
			}
		}
	}

	return true;
}

Bool Minty::Operations::copy_all(Path const& from, Path const& to)
{
	try
	{
		if (!std::filesystem::exists(from))
		{
			MINTY_ERROR_FORMAT("Failed to copy_all, from \"{}\": Path does not exist.", from.generic_string());
			return false;
		}
		else if (!std::filesystem::is_directory(from))
		{
			MINTY_ERROR_FORMAT("Failed to copy_all, from \"{}\": Path is not a directory.", from.generic_string());
			return false;
		}

		// create dest if needed
		if (!std::filesystem::exists(to))
		{
			std::filesystem::create_directories(to);
		}

		// copy over all of the files
		for (auto const& entry : std::filesystem::recursive_directory_iterator(from))
		{
			// skip non-files
			if (!entry.is_regular_file()) continue;

			Path const& path = entry.path();
			Path destination = to / path.lexically_relative(from);

			// create directory if needed
			Path directory = destination.parent_path();
			if (!std::filesystem::exists(directory))
			{
				std::filesystem::create_directories(directory);
			}

			std::filesystem::copy(path, destination, std::filesystem::copy_options::overwrite_existing);
		}
	}
	catch (std::filesystem::filesystem_error& e)
	{
		MINTY_ERROR_FORMAT("Failed to copy_all \"{}\" to \"{}\": \"{}\"", from.generic_string(), to.generic_string(), e.what());
		return false;
	}
	catch (std::exception& e)
	{
		MINTY_ERROR_FORMAT("Failed to copy_all \"{}\" to \"{}\": \"{}\"", from.generic_string(), to.generic_string(), e.what());
		return false;
	}

	return true;
}

Bool Minty::Operations::copy_some(Path const& from, Path const& to, std::unordered_set<String> const& names)
{
	try
	{
		if (!std::filesystem::exists(from))
		{
			MINTY_ERROR_FORMAT("Failed to copy_some, from \"{}\": Path does not exist.", from.generic_string());
			return false;
		}
		else if (!std::filesystem::is_directory(from))
		{
			MINTY_ERROR_FORMAT("Failed to copy_some, from \"{}\": Path is not a directory.", from.generic_string());
			return false;
		}

		// create dest if needed
		if (!std::filesystem::exists(to))
		{
			std::filesystem::create_directories(to);
		}

		// copy over all of the files
		for (auto const& entry : std::filesystem::recursive_directory_iterator(from))
		{
			// skip non-files
			if (!entry.is_regular_file()) continue;

			Path const& path = entry.path();

			// skip any files not in names set
			if (!names.contains(path.stem().string()))
			{
				continue;
			}

			Path destination = to / path.lexically_relative(from);

			// create directory if needed
			Path directory = destination.parent_path();
			if (!std::filesystem::exists(directory))
			{
				std::filesystem::create_directories(directory);
			}

			std::filesystem::copy(path, destination, std::filesystem::copy_options::overwrite_existing);
		}
	}
	catch (std::filesystem::filesystem_error& e)
	{
		MINTY_ERROR_FORMAT("Failed to copy_some \"{}\" to \"{}\": \"{}\"", from.generic_string(), to.generic_string(), e.what());
		return false;
	}
	catch (std::exception& e)
	{
		MINTY_ERROR_FORMAT("Failed to copy_some \"{}\" to \"{}\": \"{}\"", from.generic_string(), to.generic_string(), e.what());
		return false;
	}

	return true;
}

String Minty::Operations::get_environment_variable(String const& name)
{
	char* buffer = nullptr;
	Size bufferSize = 0;

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

void Minty::Operations::set_environment_variable(String const& name, String const& value)
{
#if defined(MINTY_WINDOWS)
	String temp = std::format("{}={}", name, value);
	_putenv(temp.c_str());
#else
	MINTY_NOT_IMPLEMENTED();
#endif
}

Path Minty::Operations::get_minty_path()
{
	return get_environment_variable("MINTY_PATH");
}