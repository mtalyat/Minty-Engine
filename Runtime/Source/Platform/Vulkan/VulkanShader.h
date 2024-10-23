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

		struct Frame
		{
			std::vector<Owner<VulkanBuffer>> buffers;
			VkDescriptorSet descriptorSet;
		};

	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		std::unordered_map<UInt, Binding> m_bindings;

		VkDescriptorSetLayout m_descriptorSetLayout;
		std::unordered_map<String, Size> m_descriptorNameToBufferIndex;
		std::vector<Frame> m_frames;

		VkDescriptorPool m_descriptorPool;

	public:
		VulkanShader(const ShaderBuilder& builder);

		~VulkanShader();

	public:
		void on_bind() override;

	public:
		void set_input(const String& name, const void* const data) override;

	public:
		VkPipelineLayout get_layout() const { return m_pipelineLayout; }

		VkPipeline get_pipeline() const { return m_pipeline; }
	};
}