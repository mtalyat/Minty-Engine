#include "pch.h"
#include "VulkanImage.h"

#include "Platform/Vulkan/VulkanRenderer.h"

Minty::VulkanImage::VulkanImage(ImageBuilder const& builder)
	: Image(builder)
	, m_image(VK_NULL_HANDLE)
	, m_view(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
	, m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
	, m_ownImage(true)
	, m_size(builder.pixels.size())
{
	m_image = VulkanRenderer::create_image(static_cast<uint32_t>(builder.width), static_cast<uint32_t>(builder.height), VulkanRenderer::image_type_to_vulkan(builder.type), VulkanRenderer::format_to_vulkan(builder.format), VulkanRenderer::image_tiling_to_vulkan(builder.tiling), VulkanRenderer::image_usage_to_vulkan(builder.usage) | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

	// allocate memory for the image
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(VulkanRenderer::get_device(), m_image, &memoryRequirements);
	m_memory = VulkanRenderer::allocate_memory(memoryRequirements.size, VulkanRenderer::find_memory_type(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

	VulkanRenderer::bind_image_memory(m_image, m_memory);

	// create image view
	m_view = VulkanRenderer::create_image_view(m_image, VulkanRenderer::format_to_vulkan(builder.format), VulkanRenderer::image_aspect_to_vulkan(builder.aspect));

	// set image data
	if (!builder.pixels.empty())
	{
		set_pixels(builder.pixels.data());
	}
}

Minty::VulkanImage::VulkanImage(ImageBuilder const& builder, VkImage const image)
	: Image(builder)
	, m_image(image)
	, m_view(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
	, m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
	, m_ownImage(false)
	, m_size(0)
{
	m_view = VulkanRenderer::create_image_view(m_image, VulkanRenderer::format_to_vulkan(builder.format), VulkanRenderer::image_aspect_to_vulkan(builder.aspect));
}

Minty::VulkanImage::VulkanImage(ImageBuilder const& builder, VkImage const image, VkDeviceMemory const memory, VkImageView const imageView)
	: Image(builder)
	, m_image(image)
	, m_view(imageView)
	, m_memory(memory)
	, m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
	, m_ownImage(false)
	, m_size(0)
{}

Minty::VulkanImage::~VulkanImage()
{
	dispose();
}

void Minty::VulkanImage::resize(UInt const width, UInt const height, Format const format)
{
	MINTY_ASSERT_MESSAGE(!m_immutable, "Cannot resize an immutable image.");

	// destroy old data
	dispose();

	// update data
	m_width = width;
	m_height = height;
	m_format = format;

	// create new data
	VulkanRenderer::create_image_and_memory(m_width, m_height, VulkanRenderer::image_type_to_vulkan(m_type), VulkanRenderer::format_to_vulkan(m_format), VulkanRenderer::image_tiling_to_vulkan(m_tiling), VulkanRenderer::image_usage_to_vulkan(m_usage), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_memory);

	m_view = VulkanRenderer::create_image_view(m_image, VulkanRenderer::format_to_vulkan(m_format), VulkanRenderer::image_aspect_to_vulkan(m_aspect));

	// TODO: re-set pixels, maybe as an optional boolean argument
}

void Minty::VulkanImage::set_pixels(const Byte* const data)
{
	// if this image is not owned, it cannot be modified
	if (!m_ownImage)
	{
		MINTY_ERROR("Cannot modify an image that does not own the image data.");
		return;
	}

	if (m_immutable)
	{
		MINTY_WARN("Cannot modify an image that is immutable.");
		return;
	}

	VkFormat vulkanFormat = VulkanRenderer::format_to_vulkan(m_format);

	// create staging buffer for data transfer, and put data into it
	BufferBuilder bufferBuilder{};
	bufferBuilder.data = data;
	bufferBuilder.size = m_size;
	bufferBuilder.usage = BufferUsage::TRANSFER;
	bufferBuilder.frequent = false;
	Owner<Buffer> stagingBuffer = Buffer::create(bufferBuilder);

	// transition image so it can be transfered to
	VulkanRenderer::transition_image_layout(m_image, vulkanFormat, m_layout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// copy buffer data to image
	VulkanRenderer::copy_buffer_to_image(VulkanRenderer::get_graphics_queue(), static_cast<VkBuffer>(stagingBuffer->get_native()), m_image, m_width, m_height);

	// transition image back so it can be used for rendering
	m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	VulkanRenderer::transition_image_layout(m_image, vulkanFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout);
}

void Minty::VulkanImage::dispose()
{
	VulkanRenderer::destroy_image_view(m_view);

	// only destroy the image if this Image owns it
	if (m_ownImage)
	{
		VulkanRenderer::free_memory(m_memory);
		VulkanRenderer::destroy_image(m_image);
	}
}
