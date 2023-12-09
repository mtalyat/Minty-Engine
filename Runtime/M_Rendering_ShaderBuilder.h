#pragma once
#include "M_Base.h"

#include "M_Register.h"
#include "M_Rendering_PushConstantInfo.h"
#include "M_Rendering_UniformConstantInfo.h"

#include "vulkan.h"
#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	struct ShaderBuilder
	{
		std::string name;
		std::unordered_map<std::string, PushConstantInfo> pushConstantInfos;
		std::unordered_map<std::string, UniformConstantInfo> uniformConstantInfos;

		std::vector<VkDescriptorSetLayoutBinding> get_descriptor_set_layout_bindings(uint32_t const set) const;

		uint32_t get_uniform_constant_count(uint32_t const set) const;
	};
}