#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// The base type for most Minty classes.
	/// </summary>
	class Object
	{
	public:
		virtual ~Object() {}

	public:
		friend std::string to_string(Object const& value);
	};
}

