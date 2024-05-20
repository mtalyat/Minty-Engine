#pragma once
#include "assets/M_Asset.h"

#include "libraries/M_Vulkan.h"

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
	private:
		VkBuffer _buffer;
		VkDeviceMemory _memory;
		VkDeviceSize _size;

	public:
		Buffer();

		Buffer(BufferBuilder const& builder, Runtime& runtime);

		~Buffer();

	public:
		void destroy();

		VkBuffer get_buffer() const;

		VkDeviceMemory get_memory() const;

		VkDeviceSize get_size() const;
	};
}