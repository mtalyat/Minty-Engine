#include "pch.h"
#include "VulkanRenderTarget.h"

#include "Minty/Render/RenderPass.h"
#include "Platform/Vulkan/VulkanImage.h"
#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanRenderTarget::VulkanRenderTarget(RenderTargetBuilder const& builder)
	: RenderTarget(builder)
	, m_framebuffers()
	, m_size()
{
	// initialize
	initialize(builder);
}

Minty::VulkanRenderTarget::~VulkanRenderTarget()
{
	// shutdown
	shutdown();
}

void Minty::VulkanRenderTarget::initialize(RenderTargetBuilder const& builder)
{
	VkExtent2D swapchainExtent = VulkanRenderer::get_swapchain_extent();
	m_size = { swapchainExtent.width, swapchainExtent.height };
	VkRenderPass renderPass = static_cast<VkRenderPass>(builder.renderPass->get_native());

	// create a frame buffer for each given image
	m_framebuffers.reserve(builder.images.size());
	for (Ref<Image> const image : builder.images)
	{
		Ref<VulkanImage> vulkanImage = static_cast<Ref<VulkanImage>>(image);
		m_framebuffers.push_back(VulkanRenderer::create_framebuffer(renderPass, vulkanImage->get_view(), swapchainExtent, builder.renderPass->using_color_attachment(), builder.renderPass->using_depth_attachment()));
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

void Minty::VulkanRenderTarget::reinitialize(RenderTargetBuilder const& builder)
{
	shutdown();
	initialize(builder);
}
