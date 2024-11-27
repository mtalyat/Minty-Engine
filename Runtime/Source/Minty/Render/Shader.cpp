#include "pch.h"
#include "Shader.h"

#include "Minty/Render/Renderer.h"
#include "Minty/Render/Material.h"

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

void Minty::Shader::set_global_input(String const& name, void const* const data)
{
	for (Material* const material : m_materials)
	{
		material->set_input(name, data);
	}
}

void Minty::Shader::register_material(Material& material)
{
	m_materials.emplace(&material);
}

void Minty::Shader::unregister_material(Material& material)
{
	m_materials.erase(&material);
}

Owner<Shader> Minty::Shader::create(ShaderBuilder const& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanShader>(builder);
#else
	return nullptr;
#endif
}
