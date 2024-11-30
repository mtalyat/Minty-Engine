#include "pch.h"
#include "VulkanScissor.h"

using namespace Minty;

Minty::VulkanScissor::VulkanScissor(ScissorBuilder const& builder)
	: Scissor(builder)
	, m_rect({
		{ builder.x, builder.y },
		{ builder.width, builder.height }
		})
{}

void Minty::VulkanScissor::set_x(Float const x)
{
	m_rect.offset.x = static_cast<int32_t>(x);
}

void Minty::VulkanScissor::set_y(Float const y)
{
	m_rect.offset.y = static_cast<int32_t>(y);
}

void Minty::VulkanScissor::set_width(Float const width)
{
	m_rect.extent.width = static_cast<uint32_t>(width);
}

void Minty::VulkanScissor::set_height(Float const height)
{
	m_rect.extent.height = static_cast<uint32_t>(height);
}
