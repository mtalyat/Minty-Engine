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
