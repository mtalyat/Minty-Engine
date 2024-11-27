#include "pch.h"
#include "Buffer.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanBuffer.h"
#endif

using namespace Minty;

Owner<Buffer> Minty::Buffer::create(BufferBuilder const& builder)
{
#if defined(MINTY_VULKAN)
    return Owner<VulkanBuffer>(builder);
#else
    return nullptr;
#endif
}
