#include "pch.h"
#include "Buffer.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanBuffer.h"
#endif

using namespace Minty;

Owner<Buffer> Minty::Buffer::create(const BufferBuilder& builder)
{
#if defined(MINTY_VULKAN)
    return Owner<VulkanBuffer>(builder);
#else
    return nullptr;
#endif
}
