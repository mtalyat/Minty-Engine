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
		VkRect2D const& get_rect() const { return m_rect; }
	};
}