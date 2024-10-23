#include "pch.h"
#include "Viewport.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanViewport.h"
#endif

using namespace Minty;

Owner<Viewport> Minty::Viewport::create(const ViewportBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanViewport>(builder);
#else
	return nullptr;
#endif
}
