#include "pch.h"
#include "Texture.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanTexture.h"
#endif

using namespace Minty;

Owner<Texture> Minty::Texture::create(TextureBuilder const& builder)
{
#if defined(MINTY_VULKAN)
    return Owner<VulkanTexture>(builder);
#else
    return nullptr;
#endif
}
