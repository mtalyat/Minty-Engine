#pragma once
#include "M_Base.h"

#include "M_Dynamic.h"

#include <vector>
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Holds data to create a new MaterialTemplate.
	/// </summary>
	struct MaterialTemplateBuilder
	{
		/// <summary>
		/// The name of the MaterialTemplate.
		/// </summary>
		String name;

		/// <summary>
		/// The IDs of the ShaderPasses.
		/// </summary>
		std::vector<ID> shaderPassIds;

		/// <summary>
		/// The default values.
		/// </summary>
		std::unordered_map<String, Dynamic> defaultValues;
	};
}