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