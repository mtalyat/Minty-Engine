#include "pch.h"
#include "Image.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanImage.h"
#endif

using namespace Minty;

Minty::Image::Image(ImageBuilder const& builder)
	: m_format(builder.format)
	, m_type(builder.type)
	, m_tiling(builder.tiling)
	, m_aspect(builder.aspect)
	, m_usage(builder.usage)
	, m_width(builder.width)
	, m_height(builder.height)
	, m_immutable(builder.immutable)
{
}

Owner<Image> Minty::Image::create(ImageBuilder const& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanImage>(builder);
#else
	return nullptr;
#endif
}
