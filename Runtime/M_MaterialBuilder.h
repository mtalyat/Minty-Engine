#pragma once
#include "M_Base.h"

#include "M_Dynamic.h"
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Holds data to create a new Material.
	/// </summary>
	struct MaterialBuilder
	{
		/// <summary>
		/// The name of the Material.
		/// </summary>
		String name;
		
		/// <summary>
		/// The MaterialTemplate ID.
		/// </summary>
		ID templateId;

		/// <summary>
		/// The values to set.
		/// </summary>
		std::unordered_map<String, Dynamic> values;
	};
}