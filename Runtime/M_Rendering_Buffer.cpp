#include "pch.h"
#include "M_Rendering_Buffer.h"

minty::Buffer::Buffer()
	: buffer()
	, memory()
	, size()
{}

minty::Buffer::Buffer(VkBuffer const& buffer, VkDeviceMemory const& memory, VkDeviceSize const& size)
	: buffer(buffer)
	, memory(memory)
	, size(size)
{}
