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
		VulkanScissor(const ScissorBuilder& builder);

		~VulkanScissor() = default;

	public:
		const VkRect2D& get_rect() const { return m_rect; }
	};
}