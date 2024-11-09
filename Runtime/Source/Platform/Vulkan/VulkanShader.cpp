#include "pch.h"
#include "VulkanShader.h"

#include "Minty/Core/Constants.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanTexture.h"

using namespace Minty;

Minty::VulkanShader::VulkanShader(const ShaderBuilder& builder)
	: Minty::Shader::Shader(builder)
	, m_pipelineLayout(VK_NULL_HANDLE)
	, m_pipeline(VK_NULL_HANDLE)
	, m_bindings()
	, m_descriptorSetLayout(VK_NULL_HANDLE)
	, m_descriptorNameToBufferIndex()
	, m_descriptorTypeCounts()
	, m_pools()
{
	// split up inputs from push constants
	std::vector<Minty::ShaderInput> descriptors;
	descriptors.reserve(builder.inputs.size());
	std::vector<Minty::ShaderInput> pushConstants;
	pushConstants.reserve(builder.inputs.size());

	// save inputs as well
	for (ShaderInput const& input : builder.inputs)
	{
		if (input.type == ShaderInputType::PushConstant)
		{
			pushConstants.push_back(input);
		}
		else
		{
			descriptors.push_back(input);
		}
	}

	// consolidate binding data, sum type counts, create descriptor buffers
	for (const ShaderInput& descriptor : descriptors)
	{
		MINTY_ASSERT_MESSAGE(descriptor.count != 0, "Cannot create a Descriptor with a count of 0.");
		MINTY_ASSERT_MESSAGE(!descriptor.name.empty(), "Cannot create a Descriptor with no name.");

		VkDescriptorType descriptorType = VulkanRenderer::descriptor_type_to_vulkan(descriptor.type);

		// if binding dne, make one
		auto found = m_bindings.find(descriptor.binding);

		if (found == m_bindings.end())
		{
			// create new binding based on this descriptor
			Binding binding{};
			binding.descriptorType = descriptorType;
			binding.descriptorCount = descriptor.count;
			binding.shaderStage = VulkanRenderer::shader_stage_to_vulkan(descriptor.stage);

			m_bindings.emplace(descriptor.binding, binding);
		}
		else
		{
			// add to existing binding

			// ensure matches other inputs in binding
			Binding& binding = found->second;

			MINTY_ASSERT_MESSAGE(descriptorType == binding.descriptorType, "Call Descriptors within a binding must have the same ShaderInputType.");
			MINTY_ASSERT_MESSAGE(VulkanRenderer::shader_stage_to_vulkan(descriptor.stage) == binding.shaderStage, "Call Descriptors within a binding must have the same ShaderStage.");

			binding.descriptorCount += descriptor.count;
		}

		// add to type count
		auto found2 = m_descriptorTypeCounts.find(descriptorType);
		if (found2 == m_descriptorTypeCounts.end())
		{
			// new type
			m_descriptorTypeCounts.emplace(descriptorType, descriptor.count);
		}
		else
		{
			// exiting type
			m_descriptorTypeCounts[found2->first] = found2->second + descriptor.count;
		}

		// add name to lookup
		if (descriptor.size > 0)
		{
			m_descriptorNameToBufferIndex.emplace(descriptor.name, m_descriptorNameToBufferIndex.size());
		}
	}

	MINTY_ASSERT(builder.vertexShaderModule.get() != nullptr);

	// vertex stage
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = static_cast<VkShaderModule>(builder.vertexShaderModule->get_native());
	vertexShaderStageInfo.pName = "main";

	MINTY_ASSERT(builder.fragmentShaderModule.get() != nullptr);

	// fragment stage
	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
	fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.module = static_cast<VkShaderModule>(builder.fragmentShaderModule->get_native());
	fragmentShaderStageInfo.pName = "main";

	// all stages
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

	// dynamic states (viewport, scissor)
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateInfo.pDynamicStates = dynamicStates.data();

	// vertex bindings and attributes
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	// vertex input bindings and attributes
	std::vector<VkVertexInputBindingDescription> vertexInputBindings;
	std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
	vertexInputBindings.resize(builder.vertexInput.bindings.size());
	for (Size i = 0; i < builder.vertexInput.bindings.size(); i++)
	{
		ShaderBinding const& binding = builder.vertexInput.bindings.at(i);
		VkVertexInputBindingDescription& vertexInputBinding = vertexInputBindings.at(i);
		vertexInputBinding.binding = static_cast<uint32_t>(i);
		vertexInputBinding.inputRate = VulkanRenderer::input_rate_to_vulkan(binding.rate);

		uint32_t offset = 0;

		// create each attribute as well
		for (Size j = 0; j < binding.attributes.size(); j++)
		{
			ShaderAttribute const& attribute = binding.attributes.at(j);
			VkVertexInputAttributeDescription vertexInputAttribute{};

			// treat matrices as multiple vectors
			// all others have their own format
			switch (attribute.type)
			{
			case Type::Matrix2:
				MINTY_NOT_IMPLEMENTED();
				break;
			case Type::Matrix3:
				MINTY_NOT_IMPLEMENTED();
				break;
			case Type::Matrix4:
			{
				uint32_t typeSize = static_cast<uint32_t>(sizeof(Float4));
				VkFormat format = VulkanRenderer::type_to_vulkan(Type::Float4);

				for (Size k = 0; k < 4; k++)
				{
					vertexInputAttribute.location = static_cast<uint32_t>(j + k);
					vertexInputAttribute.binding = vertexInputBinding.binding;
					vertexInputAttribute.format = format;
					vertexInputAttribute.offset = offset;

					offset += typeSize;

					vertexInputAttributes.push_back(vertexInputAttribute);
				}

				j += 4;

				break;
			}
			default:
			{
				vertexInputAttribute.location = static_cast<uint32_t>(j);
				vertexInputAttribute.binding = vertexInputBinding.binding;
				vertexInputAttribute.format = VulkanRenderer::type_to_vulkan(attribute.type);
				vertexInputAttribute.offset = offset;

				offset += static_cast<uint32_t>(sizeof_type(attribute.type));

				vertexInputAttributes.push_back(vertexInputAttribute);
				break;
			}
			}
		}

		// final offset is effectively the stride
		vertexInputBinding.stride = offset;
	}

	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
	vertexInputInfo.pVertexBindingDescriptions = vertexInputBindings.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
	vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

	// topology (point, line, or triangle)
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VulkanRenderer::primitive_topology_to_vulkan(builder.primitiveTopology);
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// viewport and scissor
	const VkViewport* viewport = &static_cast<Ref<VulkanViewport>>(builder.viewport)->get_viewport();
	const VkRect2D* scissor = &static_cast<Ref<VulkanScissor>>(builder.scissor)->get_rect();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = scissor;

	// rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
	rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerInfo.depthClampEnable = VK_FALSE; // discard pixels outside of viewport when false, otherwise clamp
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE; // geometry will pass through rasterizer when false
	rasterizerInfo.polygonMode = VulkanRenderer::polygon_mode_to_vulkan(builder.polygonMode); // how polygons are rendered (anything other than fill requires a GPU feature)
	rasterizerInfo.lineWidth = builder.lineWidth; // width of line in line/point polygon mode
	rasterizerInfo.cullMode = VulkanRenderer::cull_mode_to_vulkan(builder.cullMode); // which side to ignore
	rasterizerInfo.frontFace = VulkanRenderer::front_face_to_vulkan(builder.frontFace); // how to determine front side
	rasterizerInfo.depthBiasEnable = VK_FALSE;
	rasterizerInfo.depthBiasConstantFactor = 0.0f;
	rasterizerInfo.depthBiasClamp = 0.0f;
	rasterizerInfo.depthBiasSlopeFactor = 0.0f;

	// multisampling
	VkPipelineMultisampleStateCreateInfo multisampleInfo{};
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = nullptr;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;

	// depth stencil testing
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	// replace closer pixels
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	// only keep within a specific range (0 to 1 is default)
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	// operations
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	// color blending (transparency)
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	/*
	// transparency:
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	*/

	VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachment;
	colorBlendInfo.blendConstants[0] = 0.0f;
	colorBlendInfo.blendConstants[1] = 0.0f;
	colorBlendInfo.blendConstants[2] = 0.0f;
	colorBlendInfo.blendConstants[3] = 0.0f;

	// descriptor set layout
	std::vector<VkDescriptorSetLayoutBinding> descriptorSetBindings(m_bindings.size());
	for (Size i = 0; i < m_bindings.size(); i++)
	{
		// create binding
		VkDescriptorSetLayoutBinding& binding = descriptorSetBindings.at(i);

		uint32_t bindingIndex = static_cast<UInt>(i);

		// find shader binding info
		MINTY_ASSERT_FORMAT(m_bindings.contains(bindingIndex), "Missing ShaderBinding for binding: {}.", i);

		const Binding& bindingInfo = m_bindings.at(bindingIndex);

		binding.binding = bindingIndex;
		binding.descriptorType = bindingInfo.descriptorType;
		binding.descriptorCount = bindingInfo.descriptorCount;
		binding.stageFlags = bindingInfo.shaderStage;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(descriptorSetBindings.size());
	layoutInfo.pBindings = descriptorSetBindings.data();

	// TODO: utility/VK_ASSERT_RESULT
	if (vkCreateDescriptorSetLayout(VulkanRenderer::get_device(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout.");
	}

	// push constants
	std::vector<VkPushConstantRange> pushConstantRanges;
	pushConstantRanges.reserve(pushConstants.size());

	{
		uint32_t offset = 0;
		for (ShaderInput const& descriptor : pushConstants)
		{
			// create range for this push constant
			VkPushConstantRange pushConstantRange{};
			pushConstantRange.offset = offset;
			pushConstantRange.size = static_cast<uint32_t>(descriptor.size);
			pushConstantRange.stageFlags = VulkanRenderer::shader_stage_to_vulkan(descriptor.stage);
			pushConstantRanges.push_back(pushConstantRange);

			// increment the offset for the rest of these
			offset += static_cast<uint32_t>(descriptor.size);
		}
	}

	// layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

	// create layout
	// TODO: use VK_ASSERT_RESULT
	if (vkCreatePipelineLayout(VulkanRenderer::get_device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout.");
	}

	// create pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisampleInfo;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlendInfo;
	pipelineInfo.pDynamicState = &dynamicStateInfo;

	pipelineInfo.layout = m_pipelineLayout;

	pipelineInfo.renderPass = VulkanRenderer::get_render_pass();
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	// create the pipeline
	// TODO: use VK_ASSERT_RESULT
	if (vkCreateGraphicsPipelines(VulkanRenderer::get_device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline.");
	}
}

Minty::VulkanShader::~VulkanShader()
{
	// TODO: utility

	VkDevice device = VulkanRenderer::get_device();

	for (auto const& data : m_pools)
	{
		vkDestroyDescriptorPool(device, data.pool, nullptr);
	}
	vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
	vkDestroyPipeline(device, m_pipeline, nullptr);
	vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
}

void Minty::VulkanShader::on_bind()
{
	VkCommandBuffer commandBuffer = VulkanRenderer::get_command_buffer();

	// bind pipeline
	VulkanRenderer::bind_pipeline(commandBuffer, m_pipeline);

	// bind viewport
	Ref<VulkanViewport> vulkanViewport = static_cast<Ref<VulkanViewport>>(get_viewport());
	VulkanRenderer::bind_viewport(commandBuffer, vulkanViewport->get_viewport());

	// bind scissor
	Ref<VulkanScissor> vulkanScissor = static_cast<Ref<VulkanScissor>>(get_scissor());
	VulkanRenderer::bind_scissor(commandBuffer, vulkanScissor->get_rect());
}

static constexpr UInt DESCRIPTOR_POOL_SIZE = 20;

VkDescriptorPool Minty::VulkanShader::get_descriptor_pool(UInt const requestedSlots)
{
	// error if requesting more than able to allocate in one pool
	MINTY_ASSERT_FORMAT(requestedSlots <= DESCRIPTOR_POOL_SIZE, "Unable to create a pool that can fulfil {} slots. The maximum number of slots is {}.", requestedSlots, DESCRIPTOR_POOL_SIZE);

	// if no pools, create one
	// if last pool does not have enough space, make a new one
	// otherwise use last one
	if (m_pools.empty() || m_pools.back().slotsUsed + requestedSlots > DESCRIPTOR_POOL_SIZE)
	{
		PoolData& data = create_descriptor_pool();
		data.slotsUsed = requestedSlots;
		return data.pool;
	}
	else
	{
		PoolData& data = m_pools.back();
		data.slotsUsed += requestedSlots;
		return data.pool;
	}
}

Minty::VulkanShader::PoolData& Minty::VulkanShader::create_descriptor_pool()
{
	// scale by frames in flight and pool size
	UInt const scale = static_cast<UInt>(MAX_FRAMES_IN_FLIGHT) * DESCRIPTOR_POOL_SIZE;

	// create pool sizes
	std::vector<VkDescriptorPoolSize> descriptorPoolSizes;
	descriptorPoolSizes.reserve(m_descriptorTypeCounts.size());
	for (const auto& [type, count] : m_descriptorTypeCounts)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = type;
		poolSize.descriptorCount = count * scale;
		descriptorPoolSizes.push_back(poolSize);
	}

	// create pool
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
	poolInfo.pPoolSizes = descriptorPoolSizes.data();
	poolInfo.maxSets = scale;

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(VulkanRenderer::get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool.");
	}

	// create data and add to references
	PoolData data{};
	data.pool = descriptorPool;
	m_pools.push_back(data);
	return m_pools.back();
}


