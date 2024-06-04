#pragma once

#include "Minty/Types/M_Types.h"
#include "Minty/Core/M_Macros.h"

namespace Minty
{
	/// <summary>
	/// Contains functions that perform various tasks on the current operating system.
	/// </summary>
	namespace Operations
	{
		/// <summary>
		/// Runs a system command and returns the result code.
		/// </summary>
		/// <param name="command"></param>
		/// <returns></returns>
		int system_command(String const& command);

		/// <summary>
		/// Opens the directory at the given path.
		/// </summary>
		/// <param name="path"></param>
		void open_directory(Path const& path);

		/// <summary>
		/// Opens the file or directory at the given path.
		/// </summary>
		/// <param name="path"></param>
		void open(Path const& path);

		/// <summary>
		/// Copies the file to the target folder/file path.
		/// </summary>
		/// <param name="from"></param>
		/// <param name="to"></param>
		bool copy(Path const& from, Path const& to);

		/// <summary>
		/// Copies all of the files with the given extension to the target path.
		/// </summary>
		/// <param name="from"></param>
		/// <param name="extension"></param>
		/// <param name="to"></param>
		bool copy_files(Path const& from, Path const& extension, Path const& to);

		/// <summary>
		/// Copies all of the files and directories to the target path.
		/// </summary>
		/// <param name="from"></param>
		/// <param name="to"></param>
		bool copy_all(Path const& from, Path const& to);

		/// <summary>
		/// Gets the environment variable, if it has been set.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		String get_environment_variable(String const& name);

		/// <summary>
		/// Gets the MINTY_PATH environment variable.
		/// </summary>
		/// <returns></returns>
		Path get_minty_path();
	}
}