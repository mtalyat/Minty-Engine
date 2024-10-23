#pragma once

#include "Minty/Library/Vulkan.h"
#include "Minty/Render/Viewport.h"

namespace Minty
{
	class VulkanViewport
		: public Viewport
	{
	private:
		VkViewport m_viewport;

	public:
		VulkanViewport(const ViewportBuilder& builder);

		~VulkanViewport() = default;

	public:
		const VkViewport& get_viewport() const { return m_viewport; }
	};
}