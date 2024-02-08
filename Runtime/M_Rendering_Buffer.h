#pragma once

#include "M_Object.h"

namespace minty
{
	/// <summary>
	/// Holds data for a buffer on the GPU.
	/// </summary>
	struct Buffer
		: public Object
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;

		Buffer();

		Buffer(VkBuffer const& buffer, VkDeviceMemory const& memory, VkDeviceSize const& size);
	};
}