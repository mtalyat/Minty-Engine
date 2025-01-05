#include "pch.h"
#include "VulkanTexture.h"

#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanTexture::VulkanTexture(TextureBuilder const& builder)
	: Texture(builder)
	, m_sampler(VK_NULL_HANDLE)
{
	VkFilter filter = VulkanRenderer::filter_to_vulkan(builder.filter);
	VkSamplerAddressMode addressMode = VulkanRenderer::address_mode_to_vulkan(builder.addressMode);

	m_sampler = VulkanRenderer::create_sampler(filter, filter, addressMode, VK_BORDER_COLOR_INT_OPAQUE_BLACK, builder.normalizedCoordinates);
}

Minty::VulkanTexture::~VulkanTexture()
{
	VulkanRenderer::destroy_sampler(m_sampler);
}
