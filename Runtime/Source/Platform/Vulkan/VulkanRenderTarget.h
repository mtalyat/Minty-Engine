#pragma once

#include "Minty/Library/Vulkan.h"
#include "Minty/Render/RenderTarget.h"
#include <vector>

namespace Minty
{
	class VulkanRenderTarget
		: public RenderTarget
	{
	private:
		std::vector<VkFramebuffer> m_framebuffers;
		UInt2 m_size;

	public:
		VulkanRenderTarget(RenderTargetBuilder const& builder);

		~VulkanRenderTarget();

	public:
		void reinitialize(RenderTargetBuilder const& builder) override;

		UInt2 get_size() const override { return m_size; }

	private:
		void initialize(RenderTargetBuilder const& builder);

		void shutdown();

	public:
		VkFramebuffer get_framebuffer(Size const index) const { return m_framebuffers.at(index); }
	};
}