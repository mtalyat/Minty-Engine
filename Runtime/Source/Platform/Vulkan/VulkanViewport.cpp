#include "pch.h"
#include "VulkanViewport.h"

using namespace Minty;

Minty::VulkanViewport::VulkanViewport(ViewportBuilder const& builder)
	: Viewport(builder)
	, m_viewport(
		{
			static_cast<Float>(builder.x), static_cast<Float>(builder.y),
			static_cast<Float>(builder.width), static_cast<Float>(builder.height),
			builder.minDepth, builder.maxDepth
		})
{}

void Minty::VulkanViewport::set_x(Float const x)
{
	m_viewport.x = x;
}

void Minty::VulkanViewport::set_y(Float const y)
{
	m_viewport.y = y;
}

void Minty::VulkanViewport::set_width(Float const width)
{
	m_viewport.width = width;
}

void Minty::VulkanViewport::set_height(Float const height)
{
	m_viewport.height = height;
}
