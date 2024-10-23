#pragma once
#include "Minty/Render/ShaderModule.h"

#include "Minty/Library/Vulkan.h"

namespace Minty
{
	class VulkanShaderModule
		: public ShaderModule
	{
	private:
		VkShaderModule m_module;

	public:
		VulkanShaderModule(const ShaderModuleBuilder& builder);

		~VulkanShaderModule();

	public:
		void* get_native() const override { return m_module; }
	};
}