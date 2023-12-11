#pragma once
#include "M_Rendering_Object.h"

#include "M_Constants.h"
#include "M_Register.h"
#include "M_Rendering_PushConstantInfo.h"
#include "M_Rendering_UniformConstantInfo.h"
#include "M_Rendering_DescriptorSet.h"
#include "M_Rendering_ShaderBuilder.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Holds data for a Shader.
	/// </summary>
	class Shader :
		public rendering::RendererObject
	{
	private:
		VkPipelineLayout _pipelineLayout;
		std::array<VkDescriptorSetLayout, DESCRIPTOR_SET_COUNT> _descriptorSetLayouts;
		std::unordered_map<uint32_t, std::vector<std::pair<VkDescriptorPool, uint32_t>>> _descriptorPools;

		Register<rendering::PushConstantInfo> _pushConstantInfos;
		Register<rendering::UniformConstantInfo> _uniformConstantInfos;
		rendering::DescriptorSet _descriptorSet;
	public:
		/// <summary>
		/// Creates a new Shader with the given layout and pipeline.
		/// </summary>
		/// <param name="layout"></param>
		/// <param name="pipeline"></param>
		Shader(rendering::ShaderBuilder const& builder, Renderer& renderer);

		void destroy();

		VkPipelineLayout get_pipeline_layout() const;

		rendering::DescriptorSet const& get_global_descriptor_set() const;

		void update_push_constant(VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset = 0) const;

		void update_push_constant(std::string const& name, VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset = 0) const;

		void update_global_uniform_constant(std::string const& name, void* const value, uint32_t const size, uint32_t const offset = 0) const;
	private:
		void create_descriptor_set_layouts(rendering::ShaderBuilder const& builder);

		void create_pipeline_layout(rendering::ShaderBuilder const& builder);

		VkDescriptorPool create_pool(uint32_t const set);

		VkDescriptorPool get_pool(uint32_t const set, uint32_t const amount = MAX_FRAMES_IN_FLIGHT);

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> create_descriptor_sets(uint32_t const set);

	public:
		rendering::DescriptorSet create_descriptor_set(uint32_t const set);

		std::vector<rendering::UniformConstantInfo> get_uniform_constant_infos(uint32_t const set);

	public:
		friend std::string to_string(Shader const& shader);
	};
}

