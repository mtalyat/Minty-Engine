#pragma once

#include "M_Base.h"

namespace minty::rendering
{
	struct Buffer
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;
	};
}