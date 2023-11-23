#pragma once
#include "M_Object.h"

#include "M_Register.h"
#include "M_Rendering_PushConstantInfo.h"
#include "M_Rendering_UniformConstantInfo.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace minty
{
	class Renderer;
}

namespace minty::rendering
{
	class ShaderBuilder :
		public Object
	{
	private:
		std::unordered_map<std::string, PushConstantInfo> _pushConstantInfos;
		std::unordered_map<std::string, UniformConstantInfo> _uniformConstantInfos;

	public:
		ShaderBuilder();

#pragma region Set

#pragma endregion

#pragma region Get

		std::unordered_map<std::string, PushConstantInfo> const& get_push_constant_infos() const;

		std::unordered_map<std::string, UniformConstantInfo> const& get_uniform_constant_infos() const;

		std::vector<UniformConstantInfo> get_uniform_constant_infos(uint32_t const set) const;

		std::vector<VkDescriptorSetLayoutBinding> get_descriptor_set_layout_bindings(uint32_t const set) const;

		uint32_t get_uniform_constant_count() const;

		uint32_t get_uniform_constant_count(uint32_t const set) const;

#pragma endregion

#pragma region Emplace

		void emplace_push_constant(PushConstantInfo const& info);

		void emplace_uniform_constant(UniformConstantInfo const& info);

#pragma endregion

	public:
		friend std::string to_string(ShaderBuilder const& value);
	};
}