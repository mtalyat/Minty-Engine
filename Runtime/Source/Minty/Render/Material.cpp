#include "pch.h"
#include "Material.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanMaterial.h"
#endif

using namespace Minty;

Owner<Material> Minty::Material::create(const MaterialBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanMaterial>(builder);
#else
	return Owner<Material>();
#endif
}
