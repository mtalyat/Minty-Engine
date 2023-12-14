#pragma once
#include "M_Base.h"

#include "M_Dynamic.h"

#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	/// <summary>
	/// Holds data to create a new MaterialTemplate.
	/// </summary>
	struct MaterialTemplateBuilder
	{
		/// <summary>
		/// The name of the MaterialTemplate.
		/// </summary>
		std::string name;

		/// <summary>
		/// The IDs of the ShaderPasses.
		/// </summary>
		std::vector<ID> shaderPassIds;

		/// <summary>
		/// The default values.
		/// </summary>
		std::unordered_map<std::string, Dynamic> defaultValues;
	};
}