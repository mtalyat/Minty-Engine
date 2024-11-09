#pragma once
#include "Minty/Render/Buffer.h"

#include "Minty/Library/Vulkan.h"

namespace Minty
{
	class VulkanBuffer
		: public Buffer
	{
	private:
		VkDeviceSize m_size;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		void* m_mappedMemory;

	public:
		VulkanBuffer(const BufferBuilder& builder);

		~VulkanBuffer();

	public:
		void set_data(const void* const data) override;
		void get_data(void* const data) const override;
		void* data() const override;
		Size get_size() const override { return static_cast<Size>(m_size); }
		void flush() const override;
		void* get_native() const override { return m_buffer; }
		Owner<Buffer> clone() const override;
	};
}