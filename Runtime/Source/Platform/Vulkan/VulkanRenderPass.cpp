#include "pch.h"
#include "VulkanRenderPass.h"

#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanRenderPass::VulkanRenderPass(RenderPassBuilder const& builder)
	: RenderPass(builder)
	, m_renderPass(VK_NULL_HANDLE)
{
	if (builder.colorAttachment)
	{
		VkAttachmentDescription colorAttachment = VulkanRenderer::attachment_to_vulkan(*builder.colorAttachment);
		if (builder.depthAttachment)
		{
			VkAttachmentDescription depthAttachment = VulkanRenderer::attachment_to_vulkan(*builder.depthAttachment);
			m_renderPass = VulkanRenderer::create_render_pass(&colorAttachment, &depthAttachment);
		}
		else
		{
			m_renderPass = VulkanRenderer::create_render_pass(&colorAttachment, nullptr);
		}
	}
	else
	{
		if (builder.depthAttachment)
		{
			VkAttachmentDescription depthAttachment = VulkanRenderer::attachment_to_vulkan(*builder.depthAttachment);
			m_renderPass = VulkanRenderer::create_render_pass(nullptr, &depthAttachment);
		}
		else
		{
			MINTY_ABORT("Cannot create a RenderPass without any attachments.");
		}
	}

	// create command buffers
	for (Size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		m_commandBuffers[i] = VulkanRenderer::create_command_buffer();
	}
}

Minty::VulkanRenderPass::~VulkanRenderPass()
{
	// destroy command buffers
	for (Size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VulkanRenderer::destroy_command_buffer(m_commandBuffers[i]);
	}
}

VkCommandBuffer Minty::VulkanRenderPass::get_command_buffer() const
{
	return m_commandBuffers.at(VulkanRenderer::get_current_frame_index());
}
