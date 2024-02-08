#include "pch.h"
#include "M_Rendering_Buffer.h"

minty::rendering::Buffer::Buffer()
	: buffer()
	, memory()
	, size()
{}

minty::rendering::Buffer::Buffer(VkBuffer const& buffer, VkDeviceMemory const& memory, VkDeviceSize const& size)
	: buffer(buffer)
	, memory(memory)
	, size(size)
{}
