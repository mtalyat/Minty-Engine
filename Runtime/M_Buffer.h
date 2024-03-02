#pragma once
#include "M_Asset.h"

#include "M_Vulkan.h"

namespace minty
{
	struct BufferBuilder
	{
		UUID id;
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;
	};

	/// <summary>
	/// Holds data for a buffer on the GPU.
	/// </summary>
	class Buffer
		: public Asset
	{
		// TODO: make private, move buffer functions to this class
	public:
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;

	public:
		Buffer();

		Buffer(BufferBuilder const& builder, Runtime& runtime);
	};
}