#pragma once

#include "M_Types.h"

namespace minty
{
	enum class AssemblyType
	{
		/// <summary>
		/// The C# assembly for MintyEngine.
		/// </summary>
		Engine,

		/// <summary>
		/// The C# assembly for MintyEditor.
		/// </summary>
		Editor,

		/// <summary>
		/// The C# assembly for the project.
		/// </summary>
		Project,
	};

	String to_string(AssemblyType const value);
}