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
	/// <summary>
	/// Holds data to create a new Shader.
	/// </summary>
	struct ShaderBuilder
	{
		/// <summary>
		/// The name of the Shader.
		/// </summary>
		std::string name;

		/// <summary>
		/// The push constant infos.
		/// </summary>
		std::unordered_map<std::string, PushConstantInfo> pushConstantInfos;

		/// <summary>
		/// The uniform constant infos.
		/// </summary>
		std::unordered_map<std::string, UniformConstantInfo> uniformConstantInfos;

		/// <summary>
		/// Gets the descriptor set layout bindings for the given set.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		std::vector<VkDescriptorSetLayoutBinding> get_descriptor_set_layout_bindings(uint32_t const set) const;

		/// <summary>
		/// Gets the number of push constants for the given set.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		uint32_t get_uniform_constant_count(uint32_t const set) const;
	};
}