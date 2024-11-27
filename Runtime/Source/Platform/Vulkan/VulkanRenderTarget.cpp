#include "pch.h"
#include "VulkanRenderTarget.h"

#include "Platform/Vulkan/VulkanImage.h"
#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanRenderTarget::VulkanRenderTarget(RenderTargetBuilder const& builder)
	: RenderTarget::RenderTarget()
	, m_framebuffers()
	, m_size()
{
	// initialize
	initialize(builder);

	// register with api
	VulkanRenderer::register_target(this);
}

Minty::VulkanRenderTarget::~VulkanRenderTarget()
{
	// unregister with api
	VulkanRenderer::unregister_target(this);

	// shutdown
	shutdown();
}

void Minty::VulkanRenderTarget::initialize(RenderTargetBuilder const& builder)
{
	VkExtent2D swapchainExtent = VulkanRenderer::get_swapchain_extent();
	m_size = { swapchainExtent.width, swapchainExtent.height };

	// create a frame buffer for each given image
	m_framebuffers.reserve(builder.images.size());
	for (Ref<Image> const image : builder.images)
	{
		Ref<VulkanImage> vulkanImage = static_cast<Ref<VulkanImage>>(image);
		m_framebuffers.push_back(VulkanRenderer::create_framebuffer(VulkanRenderer::get_render_pass(), vulkanImage->get_view(), swapchainExtent));
	}
}

void Minty::VulkanRenderTarget::shutdown()
{
	// destroy old frame buffers
	for (auto const framebuffer : m_framebuffers)
	{
		VulkanRenderer::destroy_framebuffer(framebuffer);
	}
	m_framebuffers.clear();
}
