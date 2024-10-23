#include "pch.h"
#include "VulkanShader.h"

#include "Minty/Core/Constants.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanTexture.h"

Minty::VulkanShader::VulkanShader(const ShaderBuilder& builder)
	: Minty::Shader::Shader(builder)
	, m_pipelineLayout(VK_NULL_HANDLE)
	, m_pipeline(VK_NULL_HANDLE)
	, m_bindings()
	, m_descriptorSetLayout(VK_NULL_HANDLE)
	, m_descriptorNameToBufferIndex()
	, m_frames(MAX_FRAMES_IN_FLIGHT)
	, m_descriptorPool(VK_NULL_HANDLE)
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
	std::unordered_map<VkDescriptorType, uint32_t> descriptorTypeCounts;
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
		auto found2 = descriptorTypeCounts.find(descriptorType);

		if (found2 == descriptorTypeCounts.end())
		{
			// new type
			descriptorTypeCounts.emplace(descriptorType, descriptor.count);
		}
		else
		{
			// exiting type
			descriptorTypeCounts[found2->first] = found2->second + descriptor.count;
		}

		// create uniform buffers
		if (descriptor.size > 0)
		{
			// add name to lookup
			m_descriptorNameToBufferIndex.emplace(descriptor.name, m_descriptorNameToBufferIndex.size());

			// create one buffer for each frame in flight
			for (Size j = 0; j < MAX_FRAMES_IN_FLIGHT; j++)
			{
				// create uniform data buffers for each frame
				Frame& frame = m_frames.at(j);
				BufferBuilder bufferBuilder{};
				bufferBuilder.frequent = descriptor.frequent;
				bufferBuilder.size = descriptor.size;
				bufferBuilder.usage = BufferUsage::UNIFORM;
				bufferBuilder.data = nullptr;
				frame.buffers.push_back(Owner<VulkanBuffer>(bufferBuilder));
			}
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
		vertexInputBinding.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX; // TODO: instanced support

		uint32_t offset = 0;

		// create each attribute as well
		for (Size j = 0; j < binding.attributes.size(); j++)
		{
			ShaderAttribute const& attribute = binding.attributes.at(j);
			VkVertexInputAttributeDescription vertexInputAttribute{};
			vertexInputAttribute.location = static_cast<uint32_t>(j);
			vertexInputAttribute.binding = vertexInputBinding.binding;
			vertexInputAttribute.format = VulkanRenderer::type_to_vulkan(attribute.type);
			vertexInputAttribute.offset = offset;

			offset += static_cast<uint32_t>(sizeof_type(attribute.type));

			vertexInputAttributes.push_back(vertexInputAttribute);
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

	// create pool sizes
	std::vector<VkDescriptorPoolSize> descriptorPoolSizes;
	descriptorPoolSizes.reserve(descriptorTypeCounts.size());
	for (const auto& [type, count] : descriptorTypeCounts)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = type;
		poolSize.descriptorCount = count * MAX_FRAMES_IN_FLIGHT; // scale by frames in flight
		descriptorPoolSizes.push_back(poolSize);
	}

	// create pool
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
	poolInfo.pPoolSizes = descriptorPoolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(VulkanRenderer::get_device(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool.");
	}

	// allocate descriptor sets for each frame
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts(MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
	VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
	descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocInfo.descriptorPool = m_descriptorPool;
	descriptorSetAllocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	descriptorSetAllocInfo.pSetLayouts = descriptorSetLayouts.data();

	std::vector<VkDescriptorSet> descriptorSets(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(VulkanRenderer::get_device(), &descriptorSetAllocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets.");
	}

	// distribute to frames
	for (Size i = 0; i < m_frames.size(); i++)
	{
		m_frames[i].descriptorSet = descriptorSets.at(i);
	}

	// write to all the inputs
	for (Frame& frame : m_frames)
	{
		std::vector<VkDescriptorBufferInfo> bufferInfos;
		std::vector<VkDescriptorImageInfo> imageInfos;
		bufferInfos.reserve(descriptors.size());
		imageInfos.reserve(descriptors.size());

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.reserve(descriptors.size());

		for (const ShaderInput& descriptor : descriptors)
		{
			switch (descriptor.type)
			{
			case ShaderInputType::Sample:
			case ShaderInputType::SampledImage:
			case ShaderInputType::StorageImage:
			case ShaderInputType::UniformTexelBuffer:
			case ShaderInputType::StorageTexelBuffer:
			case ShaderInputType::UniformBufferDynamic:
			case ShaderInputType::StorageBufferDynamic:
			{
				MINTY_NOT_IMPLEMENTED();
			}
			break;
			case ShaderInputType::CombinedImageSampler:
			{
				// ignore for now
			}
			break;
			case ShaderInputType::UniformBuffer:
			case ShaderInputType::StorageBuffer:
			{
				VkWriteDescriptorSet descriptorWrite{};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = frame.descriptorSet;
				descriptorWrite.dstBinding = descriptor.binding;
				descriptorWrite.dstArrayElement = 0; // if an array

				descriptorWrite.descriptorType = VulkanRenderer::descriptor_type_to_vulkan(descriptor.type);
				descriptorWrite.descriptorCount = 1; // how many to update

				// buffer
				//Ref<VulkanBuffer> vulkanBuffer = *static_cast<Ref<VulkanBuffer>*>(descriptor.value);
				Ref<VulkanBuffer> vulkanBuffer = frame.buffers.at(m_descriptorNameToBufferIndex.at(descriptor.name));

				VkDescriptorBufferInfo bufferInfo{};
				bufferInfo.buffer = static_cast<VkBuffer>(vulkanBuffer->get_native());
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;

				bufferInfos.push_back(bufferInfo);
				descriptorWrite.pBufferInfo = &bufferInfos.back();

				descriptorWrites.push_back(descriptorWrite);

			}
			break;
			}
		}

		vkUpdateDescriptorSets(VulkanRenderer::get_device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

Minty::VulkanShader::~VulkanShader()
{
	m_frames.clear();

	// TODO: utility
	vkDestroyDescriptorPool(VulkanRenderer::get_device(), m_descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(VulkanRenderer::get_device(), m_descriptorSetLayout, nullptr);
	vkDestroyPipeline(VulkanRenderer::get_device(), m_pipeline, nullptr);
	vkDestroyPipelineLayout(VulkanRenderer::get_device(), m_pipelineLayout, nullptr);
}

void Minty::VulkanShader::on_bind()
{
	VkCommandBuffer commandBuffer = VulkanRenderer::get_command_buffer();
	Frame& frame = m_frames.at(VulkanRenderer::get_current_frame_index());

	// bind pipeline
	VulkanRenderer::bind_pipeline(commandBuffer, m_pipeline);

	// bind descriptor set(s)
	VulkanRenderer::bind_descriptor_set(commandBuffer, m_pipelineLayout, frame.descriptorSet);

	// bind viewport
	Ref<VulkanViewport> vulkanViewport = static_cast<Ref<VulkanViewport>>(get_viewport());
	VulkanRenderer::bind_viewport(commandBuffer, vulkanViewport->get_viewport());

	// bind scissor
	Ref<VulkanScissor> vulkanScissor = static_cast<Ref<VulkanScissor>>(get_scissor());
	VulkanRenderer::bind_scissor(commandBuffer, vulkanScissor->get_rect());
}

void Minty::VulkanShader::set_input(const String& name, const void* const data)
{
	MINTY_ASSERT_FORMAT(has_input(name), "\"{}\" is not the name of any constant in this Shader.", name);
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot set_input to null.");

	ShaderInput const& input = get_input(name);

	switch (input.type)
	{
	case ShaderInputType::UniformBuffer:
	case ShaderInputType::StorageBuffer:
	{
		// get index of buffer
		Size index = m_descriptorNameToBufferIndex.at(name);

		// set data in buffers
		for (Frame& frame : m_frames)
		{
			Ref<VulkanBuffer> buffer = frame.buffers.at(index);
			buffer->set_data(data);
		}
	}
	break;
	case ShaderInputType::CombinedImageSampler:
	{
		// write to all the inputs
		for (Frame& frame : m_frames)
		{
			VkDescriptorImageInfo imageInfo{};

			// set write info
			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = frame.descriptorSet;
			descriptorWrite.dstBinding = input.binding;
			descriptorWrite.dstArrayElement = 0; // if an array
			descriptorWrite.descriptorType = VulkanRenderer::descriptor_type_to_vulkan(input.type);
			descriptorWrite.descriptorCount = 1; // how many to update

			// get the texture
			VulkanTexture const* vulkanTexture = *static_cast<VulkanTexture const* const*>(data);;

			// create image info from the texture
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = vulkanTexture->get_view();
			imageInfo.sampler = vulkanTexture->get_sampler();
			descriptorWrite.pImageInfo = &imageInfo;

			// push updates
			vkUpdateDescriptorSets(VulkanRenderer::get_device(), 1, &descriptorWrite, 0, nullptr);
		}
	}
	break;
	case ShaderInputType::PushConstant:
	{
		vkCmdPushConstants(VulkanRenderer::get_command_buffer(), m_pipelineLayout, VulkanRenderer::shader_stage_to_vulkan(input.stage), input.offset, input.size, data);
	}
	break;
	}
}
