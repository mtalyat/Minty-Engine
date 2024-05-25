#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Libraries/M_Vulkan.h"

namespace Minty
{
	struct BufferBuilder
	{
		UUID id = UUID::create();
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
		Buffer() = default;

		Buffer(BufferBuilder const& builder)
			: Asset(builder.id, ""), _buffer(builder.buffer), _memory(builder.memory), _size(builder.size) {}

		~Buffer();

	public:
		void destroy();

		VkBuffer get_buffer() const { return _buffer; }

		VkDeviceMemory get_memory() const { return _memory; }

		VkDeviceSize get_size() const { return _size; }
	};
}