#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Types/M_Register.h"
#include "Minty/Rendering/M_PushConstantInfo.h"
#include "Minty/Rendering/M_UniformConstantInfo.h"
#include "Minty/Rendering/M_DescriptorSet.h"

#include "Minty/Libraries/M_Vulkan.h"
#include <array>
#include <vector>
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Holds data to create a new Shader.
	/// </summary>
	struct ShaderBuilder
	{
		UUID id;

		Path path;

		/// <summary>
		/// The push constant infos.
		/// </summary>
		std::unordered_map<String, PushConstantInfo> pushConstantInfos;

		/// <summary>
		/// The uniform constant infos.
		/// </summary>
		std::unordered_map<String, UniformConstantInfo> uniformConstantInfos;

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

	/// <summary>
	/// Holds data for a Shader.
	/// </summary>
	class Shader :
		public Asset
	{
	private:
		VkPipelineLayout _pipelineLayout;
		std::array<VkDescriptorSetLayout, DESCRIPTOR_SET_COUNT> _descriptorSetLayouts;
		std::unordered_map<uint32_t, std::unordered_map<VkDescriptorPool, uint32_t>> _descriptorPools;

		Register<PushConstantInfo> _pushConstantInfos;
		Register<UniformConstantInfo> _uniformConstantInfos;
		DescriptorSet _descriptorSet;
	public:
		/// <summary>
		/// Creates an empty Shader.
		/// </summary>
		Shader() = default;

		/// <summary>
		/// Creates a new Shader.
		/// </summary>
		/// <param name="layout"></param>
		/// <param name="pipeline"></param>
		Shader(ShaderBuilder const& builder);

		~Shader();

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
		DescriptorSet const& get_global_descriptor_set() const;

		/// <summary>
		/// Updates the push constant.
		/// </summary>
		/// <param name="commandBuffer"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_push_constant(VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset = 0);

		/// <summary>
		/// Updates the push constant with the given name.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="commandBuffer"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_push_constant(String const& name, VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset = 0);

		/// <summary>
		/// Updates the global uniform constant.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_global_uniform_constant(String const& name, void const* const value, uint32_t const size, uint32_t const offset);

		/// <summary>
		/// Updates the global uniform constant.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="frame"></param>
		/// <param name="value"></param>
		/// <param name="size"></param>
		/// <param name="offset"></param>
		void update_global_uniform_constant(String const& name, int const frame, void const* const value, uint32_t const size, uint32_t const offset);

		/// <summary>
		/// Create a descriptor set based on the given set index.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		DescriptorSet create_descriptor_set(uint32_t const set, bool const initialize);

		/// <summary>
		/// Destroys the given descriptor set.
		/// </summary>
		/// <param name="set"></param>
		void free_descriptor_set(DescriptorSet const& set);

		/// <summary>
		/// Gets the uniform constant infos for the given set index.
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		std::vector<UniformConstantInfo> get_uniform_constant_infos(uint32_t const set) const;
	private:
		void create_descriptor_set_layouts(ShaderBuilder const& builder);

		void create_pipeline_layout(ShaderBuilder const& builder);

		VkDescriptorPool create_pool(uint32_t const set);

		VkDescriptorPool take_pool(uint32_t const set, uint32_t const amount = MAX_FRAMES_IN_FLIGHT);

		void give_pool(uint32_t const set, VkDescriptorPool const& pool, uint32_t const amount = MAX_FRAMES_IN_FLIGHT);

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> create_descriptor_sets(VkDescriptorPool const& pool, uint32_t const set);

	public:
		friend String to_string(Shader const& shader);
	};
}

