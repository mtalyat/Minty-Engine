#include "pch.h"
#include "Scissor.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanScissor.h"
#endif

using namespace Minty;

Owner<Scissor> Minty::Scissor::create(const ScissorBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanScissor>(builder);
#else
	return nullptr;
#endif
}
