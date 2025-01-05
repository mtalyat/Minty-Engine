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
}
