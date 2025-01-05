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

	public:
		VulkanRenderPass(RenderPassBuilder const& builder);

		~VulkanRenderPass()
		{}

	public:
		VkRenderPass get_render_pass() const { return m_renderPass; }

	public:
		void* get_native() const override { return m_renderPass; }
	};
}