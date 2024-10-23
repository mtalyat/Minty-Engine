#include "pch.h"
#include "VulkanViewport.h"

using namespace Minty;

Minty::VulkanViewport::VulkanViewport(const ViewportBuilder& builder)
	: Viewport(builder)
	, m_viewport(
		{
			static_cast<Float>(builder.x), static_cast<Float>(builder.y),
			static_cast<Float>(builder.width), static_cast<Float>(builder.height),
			builder.minDepth, builder.maxDepth
		})
{}
