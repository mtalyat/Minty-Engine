#pragma once
#include "M_Base.h"

#include "M_Dynamic.h"
#include <unordered_map>

namespace minty::rendering
{
	struct MaterialBuilder
	{
		std::string name;
		ID templateId;

		std::unordered_map<std::string, Dynamic> values;
	};
}