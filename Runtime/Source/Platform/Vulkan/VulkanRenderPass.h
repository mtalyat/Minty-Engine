#pragma once
#include "Minty/Render/RenderPass.h"

#include "Minty/Core/Constants.h"
#include "Minty/Library/Vulkan.h"
#include <array>

namespace Minty
{
	class VulkanRenderPass
		: public RenderPass
	{
	private:
		VkRenderPass m_renderPass;
		std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> m_commandBuffers;

	public:
		VulkanRenderPass(RenderPassBuilder const& builder);

		~VulkanRenderPass();

	public:
		VkRenderPass get_render_pass() const { return m_renderPass; }

		VkCommandBuffer get_command_buffer() const;

	public:
		void* get_native() const override { return m_renderPass; }
	};
}