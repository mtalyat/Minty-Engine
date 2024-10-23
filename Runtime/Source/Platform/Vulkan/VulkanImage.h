#pragma once
#include "Minty/Render/Image.h"

#include "Minty/Core/Types.h"
#include "Minty/Library/Vulkan.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Minty
{
	class VulkanImage
		: public Image
	{
		VkImage m_image;
		VkImageView m_view;
		VkDeviceMemory m_memory;
		Format m_format;
		VkImageLayout m_layout;

		Bool m_ownImage;

		UInt m_width;
		UInt m_height;
		Size m_size;
		Bool m_immutable;

	public:
		VulkanImage(const ImageBuilder& builder);

		VulkanImage(const ImageBuilder& builder, const VkImage image);
		
		VulkanImage(const ImageBuilder& builder, const VkImage image, const VkDeviceMemory memory, const VkImageView imageView);

		~VulkanImage();

		void set_pixels(const Byte* const data) override;

		void* get_native() const override { return m_image; }

		UInt get_width() const override { return m_width; }

		UInt get_height() const override { return m_width; }

		VkImageView get_view() const { return m_view; }
	};
}