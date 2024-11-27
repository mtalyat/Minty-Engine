#include "pch.h"
#include "Image.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanImage.h"
#endif

using namespace Minty;

Owner<Image> Minty::Image::create(ImageBuilder const& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanImage>(builder);
#else
	return nullptr;
#endif
}
