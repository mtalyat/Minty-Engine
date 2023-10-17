#pragma once
#include "M_Rendering_Object.h"

#include "M_Constants.h"
#include "M_Register.h"

#include <vulkan/vulkan.h>

namespace minty
{
	namespace rendering
	{
		class ShaderBuilder;
	}

	/// <summary>
	/// Holds data for a Shader.
	/// </summary>
	class Shader :
		public rendering::RendererObject
	{
	private:
		struct PushConstantInfo
		{
			VkShaderStageFlags flags;
		};

		struct UniformConstantInfo
		{
			
		};

		VkPipelineLayout _layout;
		VkPipeline _pipeline;

		uint32_t _uniformCount;

		std::vector<VkBuffer> _buffers;
		std::vector<VkDeviceMemory> _memories;
		std::vector<void*> _mapped;

		std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;
		VkDescriptorPool _descriptorPool;
		std::vector<VkDescriptorSet> _descriptorSets;

		minty::Register<PushConstantInfo> _pushConstants;
		minty::Register<UniformConstantInfo> _uniformConstants;
	public:
		/// <summary>
		/// Creates a new Shader with the given layout and pipeline.
		/// </summary>
		/// <param name="layout"></param>
		/// <param name="pipeline"></param>
		Shader(std::string const& vertexPath, std::string const& fragmentPath, rendering::ShaderBuilder const& builder, Renderer& renderer);

		void destroy();

		void bind(VkCommandBuffer const commandBuffer) const;

		void update_push_constant(VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t offset = 0) const;

		void update_push_constant(std::string const& name, VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t offset = 0) const;

		/// <summary>
		/// Updates the uniform constant with the given name, for all frames.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="elementSize"></param>
		/// <param name="count"></param>
		/// <param name="index"></param>
		void update_uniform_constant(std::string const& name, void* const value, size_t const elementSize, size_t const count = 1, size_t const index = 0) const;

		/// <summary>
		/// Updates the uniform constant with the given name, only for the active frame.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="elementSize"></param>
		/// <param name="count"></param>
		/// <param name="index"></param>
		void update_uniform_constant_frame(std::string const& name, void* const value, size_t const elementSize, size_t const count = 1, size_t const index = 0) const;
	private:

		size_t get_buffer_index(size_t const buffer, size_t const frame) const;

		size_t get_buffer_count() const;

		VkShaderModule load_module(std::string const& path);

		void create_descriptor_set_layouts(rendering::ShaderBuilder const& builder);

		void create_shader(std::string const& vertexPath, std::string const& fragmentPath, rendering::ShaderBuilder const& builder);

		void create_buffers(rendering::ShaderBuilder const& builder);

		void create_descriptor_pool(rendering::ShaderBuilder const& builder);

		void create_descriptor_sets(rendering::ShaderBuilder const& builder);
	};
}

