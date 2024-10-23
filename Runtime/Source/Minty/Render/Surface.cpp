#include "pch.h"
#include "Surface.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanSurface.h"
#endif

using namespace Minty;

Owner<Surface> Minty::Surface::create(const SurfaceBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanSurface>(builder);
#else
	return nullptr;
#endif
}
