#pragma once
#include "M_Base.h"

#include "M_Dynamic.h"

#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	struct MaterialTemplateBuilder
	{
		std::string name;
		std::vector<ID> shaderPassIds;

		std::unordered_map<std::string, Dynamic> defaultValues;
	};
}