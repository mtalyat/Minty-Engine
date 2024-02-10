#pragma once

#include "M_Types.h"
#include "M_Macros.h"

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
	}
}