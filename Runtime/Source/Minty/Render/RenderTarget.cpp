#include "pch.h"
#include "RenderTarget.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanRenderTarget.h"
#endif

using namespace Minty;

Owner<RenderTarget> Minty::RenderTarget::create(const RenderTargetBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanRenderTarget>(builder);
#else
	return nullptr;
#endif
}
