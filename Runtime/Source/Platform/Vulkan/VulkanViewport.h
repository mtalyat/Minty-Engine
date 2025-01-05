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
		VulkanViewport(ViewportBuilder const& builder);

		~VulkanViewport() = default;

	public:
		void set_x(Float const x) override;

		void set_y(Float const y) override;

		void set_width(Float const width) override;

		void set_height(Float const height) override;

	public:
		VkViewport const& get_viewport() const { return m_viewport; }
	};
}