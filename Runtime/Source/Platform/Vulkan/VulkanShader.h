#pragma once
#include "Minty/Render/Shader.h"

#include "Minty/Library/Vulkan.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	class VulkanShader
		: public Shader
	{
	private:
		struct Binding
		{
			VkShaderStageFlags shaderStage;
			VkDescriptorType descriptorType;
			uint32_t descriptorCount;
		};

		struct PoolData
		{
			VkDescriptorPool pool;
			UInt slotsUsed = 0;
		};

	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		std::unordered_map<UInt, Binding> m_bindings;

		VkDescriptorSetLayout m_descriptorSetLayout;
		std::unordered_map<String, Size> m_descriptorNameToBufferIndex;
		std::unordered_map<VkDescriptorType, UInt> m_descriptorTypeCounts;

		std::vector<PoolData> m_pools;

	public:
		VulkanShader(ShaderBuilder const& builder);

		~VulkanShader();

	public:
		void on_bind() override;

	public:
		VkPipelineLayout get_layout() const { return m_pipelineLayout; }

		VkPipeline get_pipeline() const { return m_pipeline; }

		VkDescriptorSetLayout get_descriptor_set_layout() const { return m_descriptorSetLayout; }

		VkDescriptorPool get_descriptor_pool(UInt const requestedSlots);

		Bool contains_descriptor(String const& name) const { return m_descriptorNameToBufferIndex.contains(name); }

		Size get_descriptor_buffer_index(String const& name) const { return m_descriptorNameToBufferIndex.at(name); }

	private:
		PoolData& create_descriptor_pool();
	};
}