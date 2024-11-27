#include "pch.h"
#include "VulkanTexture.h"

#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanTexture::VulkanTexture(TextureBuilder const& builder)
	: Texture(builder)
	, m_sampler(VK_NULL_HANDLE)
{
	m_sampler = VulkanRenderer::create_sampler();
}

Minty::VulkanTexture::~VulkanTexture()
{
	VulkanRenderer::destroy_sampler(m_sampler);
}
