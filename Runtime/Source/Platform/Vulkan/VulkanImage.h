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
		VkImageLayout m_layout;

		// if true, dispose image resources when done with it
		Bool m_ownImage;
		
		// size in pixels
		Size m_size;

	public:
		VulkanImage(ImageBuilder const& builder);

		VulkanImage(ImageBuilder const& builder, VkImage const image);
		
		VulkanImage(ImageBuilder const& builder, VkImage const image, VkDeviceMemory const memory, VkImageView const imageView);

		~VulkanImage();

	public:
		void resize(UInt const width, UInt const height, Format const format) override;

		void set_pixels(const Byte* const data) override;

		void* get_native() const override { return m_image; }

		VkImageView get_view() const { return m_view; }

	private:
		// frees all vulkan assets
		void dispose();
	};
}