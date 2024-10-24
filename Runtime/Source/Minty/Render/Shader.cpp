#include "pch.h"
#include "Shader.h"

#include "Minty/Render/Renderer.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanShader.h"
#endif

using namespace Minty;

std::vector<ShaderInput> Minty::Shader::get_inputs() const
{
	std::vector<ShaderInput> inputs(m_inputs.size());

	Size i = 0;
	for (auto const& [name, input] : m_inputs)
	{
		inputs.at(i) = input;

		i++;
	}

	return inputs;
}

Owner<Shader> Minty::Shader::create(const ShaderBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanShader>(builder);
#else
	return nullptr;
#endif
}
