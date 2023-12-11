#pragma once

#include "M_Base.h"

#include <vector>

namespace minty::rendering
{
	struct UniformConstantInfo
	{
		std::string name;
		VkDescriptorType type;
		uint32_t set;
		uint32_t binding;
		uint32_t count;
		VkDeviceSize size;
		VkShaderStageFlags stageFlags;
		std::vector<ID> ids;
	};
}