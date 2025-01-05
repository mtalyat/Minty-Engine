#pragma once

#include "Minty/Library/Vulkan.h"
#include "Minty/Render/Scissor.h"

namespace Minty
{
	class VulkanScissor
		: public Scissor
	{
	private:
		VkRect2D m_rect;

	public:
		VulkanScissor(ScissorBuilder const& builder);

		~VulkanScissor() = default;

	public:
		void set_x(Float const x) override;

		void set_y(Float const y) override;

		void set_width(Float const width) override;

		void set_height(Float const height) override;

	public:
		VkRect2D const& get_rect() const { return m_rect; }
	};
}