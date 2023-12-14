#pragma once

#include "M_Base.h"

namespace minty::rendering
{
	/// <summary>
	/// Holds data for a buffer on the GPU.
	/// </summary>
	struct Buffer
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;
	};
}