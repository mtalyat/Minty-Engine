#include "pch.h"
#include "VulkanShaderModule.h"

#include "Minty/File/File.h"
#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanShaderModule::VulkanShaderModule(const ShaderModuleBuilder& builder)
	: Minty::ShaderModule::ShaderModule(builder)
	, m_module(nullptr)
{
	// create shader module
	m_module = VulkanRenderer::create_shader_module(builder.path);
}

Minty::VulkanShaderModule::~VulkanShaderModule()
{
	// destroy shader module
	VulkanRenderer::destroy_shader_module(m_module);
}