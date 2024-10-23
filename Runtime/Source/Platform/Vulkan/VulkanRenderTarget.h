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
		VulkanRenderTarget(const RenderTargetBuilder& builder);

		~VulkanRenderTarget();

	public:
		void initialize(const RenderTargetBuilder& builder);

		void shutdown();

	public:
		UInt2 get_size() const override { return m_size; }

	public:
		VkFramebuffer get_framebuffer(const Size index) const { return m_framebuffers.at(index); }
	};
}