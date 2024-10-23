#include "pch.h"
#include "Shader.h"

#include "Minty/Render/Renderer.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanShader.h"
#endif

using namespace Minty;

Owner<Shader> Minty::Shader::create(const ShaderBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanShader>(builder);
#else
	return nullptr;
#endif
}
