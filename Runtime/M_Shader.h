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
		public rendering::RenderObject
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
		/// Creates a new Shader.
		/// </summary>
		/// <param name="layout"></param>
		/// <param name="pipeline"></param>
		Shader(rendering::ShaderBuilder const& builder, RenderEngine& renderer);

		/// <summary>
		/// Destroys all of the resources associated with this Shader.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the pipeline layout for this Shader.
		/// </summary>
		/// <returns></returns>
		VkPipelineLayout get_pipeline_layout() const;

		/// <summary>
		/// Gets the global descriptor set for this Shader.
		/// </summary>
		/// <returns></returns>
		rendering::DescriptorSet const& get_global_descriptor_set() const;

		/// <summary>
		/// Updates the push constant.
		/// </summary>
		/// <param name="commandBuffer"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_push_constant(VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset = 0) const;

		/// <summary>
		/// Updates the push constant with the given name.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="commandBuffer"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_push_constant(String const& name, VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset = 0) const;

		/// <summary>
		/// Updates the global uniform constant.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_global_uniform_constant(String const& name, void* const value, uint32_t const size, uint32_t const offset = 0) const;

		/// <summary>
		/// Create a descriptor set based on the given set index.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		rendering::DescriptorSet create_descriptor_set(uint32_t const set);

		/// <summary>
		/// Gets the uniform constant infos for the given set index.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		std::vector<rendering::UniformConstantInfo> get_uniform_constant_infos(uint32_t const set);
	private:
		void create_descriptor_set_layouts(rendering::ShaderBuilder const& builder);

		void create_pipeline_layout(rendering::ShaderBuilder const& builder);

		VkDescriptorPool create_pool(uint32_t const set);

		VkDescriptorPool get_pool(uint32_t const set, uint32_t const amount = MAX_FRAMES_IN_FLIGHT);

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> create_descriptor_sets(uint32_t const set);

	public:
		friend String to_string(Shader const& shader);
	};
}

