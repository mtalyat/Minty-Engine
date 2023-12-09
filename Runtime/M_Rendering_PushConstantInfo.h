#pragma once

#include "M_Base.h"

namespace minty::rendering
{
	struct PushConstantInfo
	{
		std::string name;
		VkShaderStageFlags stageFlags;
		uint32_t offset;
		uint32_t size;
	};
}