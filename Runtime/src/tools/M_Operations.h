#pragma once

#include "types/M_Types.h"
#include "core/M_Macros.h"

namespace minty
{
	/// <summary>
	/// Contains functions that perform various tasks on the current operating system.
	/// </summary>
	namespace Operations
	{
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

		String get_environment_variable(String const& name);

		Path get_minty_path();
	}
}