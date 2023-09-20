#pragma once

#include "M_Types.h"
#include <string>
#include <format>

namespace minty
{
	/// <summary>
	/// The base type for most Minty classes.
	/// </summary>
	class Object
	{
	public:
		virtual ~Object() {}

		/// <summary>
		/// Converts this Object to a human readable string.
		/// </summary>
		/// <returns>A string of this Object.</returns>
		virtual std::string const toString() const;
	};
}

