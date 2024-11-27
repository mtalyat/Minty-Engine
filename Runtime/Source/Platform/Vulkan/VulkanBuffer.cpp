#include "pch.h"
#include "VulkanBuffer.h"

#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanBuffer::VulkanBuffer(BufferBuilder const& builder)
	: Buffer(builder)
	, m_size(static_cast<VkDeviceSize>(builder.size))
	, m_buffer(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
	, m_mappedMemory(nullptr)
{
	MINTY_ASSERT_MESSAGE(builder.size > 0, "Attempting to create a buffer with a size of 0.");

	// create this buffer, based on frequency
	int extraFlags = is_frequent() ? 0 : VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	VkMemoryPropertyFlags memoryPropertyFlags = is_frequent() ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VulkanRenderer::create_buffer_and_memory(builder.size, VulkanRenderer::buffer_usage_to_vulkan(builder.usage) | extraFlags, memoryPropertyFlags, m_buffer, m_memory);

	// map memory if frequent
	if (is_frequent())
	{
		m_mappedMemory = VulkanRenderer::map_memory(m_memory, 0, builder.size);
	}

	// set data, if given any initial data
	if (builder.data)
	{
		set_data(builder.data);
	}
}

Minty::VulkanBuffer::~VulkanBuffer()
{
	// unmap memory if frequent
	if (is_frequent())
	{
		VulkanRenderer::unmap_memory(m_memory);
	}

	VulkanRenderer::destroy_buffer(m_buffer);
	VulkanRenderer::free_memory(m_memory);
}

void Minty::VulkanBuffer::set_data(const void* const data)
{
	// if frequent: straight upload
	// if infrequent: staging buffer
	if (is_frequent())
	{
		// frequent: already mapped
		memcpy(m_mappedMemory, data, m_size);
	}
	else
	{
		// infrequent

		// create staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanRenderer::create_buffer_and_memory(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// set data into staging buffer
		VulkanRenderer::set_memory(stagingBufferMemory, data, 0, m_size);

		// copy buffer
		VulkanRenderer::copy_buffer_to_buffer(VulkanRenderer::get_graphics_queue(), stagingBuffer, m_buffer, m_size);

		VulkanRenderer::destroy_buffer_and_memory(stagingBuffer, stagingBufferMemory);
	}
}

void Minty::VulkanBuffer::get_data(void* const data) const
{
	if (is_frequent())
	{
		memcpy(data, m_mappedMemory, m_size);
	}
	else
	{
		// map data so it can be read
		void* bufferData = VulkanRenderer::map_memory(m_memory, 0, m_size);

		// copy data from it
		memcpy(data, bufferData, m_size);

		// unmap data
		VulkanRenderer::unmap_memory(m_memory);
	}
}

void* Minty::VulkanBuffer::data() const
{
	MINTY_ASSERT_MESSAGE(is_frequent(), "Cannot use data() on a Buffer that does not have frequent set to true.");

	return m_mappedMemory;
}

void Minty::VulkanBuffer::flush() const
{
	MINTY_NOT_IMPLEMENTED();
}

Owner<Buffer> Minty::VulkanBuffer::clone() const
{
	// get a copy of the data
	void* data = new Byte[m_size];
	get_data(data);

	BufferBuilder builder{};
	builder.data = data;
	builder.frequent = is_frequent();
	builder.size = m_size;
	builder.usage = get_usage();

	Owner<VulkanBuffer> result = Owner<VulkanBuffer>(builder);

	// delete the copy
	delete[] data;

	return result;
}
