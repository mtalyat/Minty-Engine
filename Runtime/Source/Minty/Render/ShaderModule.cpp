#include "pch.h"
#include "ShaderModule.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanShaderModule.h"
#endif

using namespace Minty;

Owner<ShaderModule> Minty::ShaderModule::create(ShaderModuleBuilder const& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanShaderModule>(builder);
#else
	return nullptr;
#endif
}