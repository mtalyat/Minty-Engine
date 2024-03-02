#include "pch.h"
#include "M_ShaderPass.h"

#include "M_Runtime.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"

using namespace minty;
using namespace minty;

minty::ShaderPass::ShaderPass()
	: Asset()
	, _shader()
	, _pipeline()
	, _descriptorSet()
{}

minty::ShaderPass::ShaderPass(ShaderPassBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _shader(builder.shader)
	, _pipeline()
	, _descriptorSet(engine)
{
	MINTY_ASSERT(builder.shader != nullptr);

	create_pipeline(builder);

	_descriptorSet = _shader->create_descriptor_set(DESCRIPTOR_SET_SHADER_PASS, true);
}

void minty::ShaderPass::destroy()
{
	_shader = nullptr;
	vkDestroyPipeline(get_runtime().get_render_engine().get_device(), _pipeline, nullptr);
}

Shader* minty::ShaderPass::get_shader() const
{
	return _shader;
}

VkPipeline minty::ShaderPass::get_pipeline() const
{
	return _pipeline;
}

DescriptorSet const& minty::ShaderPass::get_descriptor_set() const
{
	return _descriptorSet;
}

void minty::ShaderPass::create_pipeline(ShaderPassBuilder const& builder)
{
	// create 
	RenderEngine& renderer = get_runtime().get_render_engine();
	VkDevice device = renderer.get_device();

	// load shader stages
	auto const& shaderStageInfos = builder.stages;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages(shaderStageInfos.size());

	for (size_t i = 0; i < shaderStages.size(); i++)
	{
		VkPipelineShaderStageCreateInfo& stage = shaderStages.at(i);
		ShaderPassBuilder::ShaderStageInfo const& info = shaderStageInfos.at(i);
		stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage.module = renderer.create_shader_module(info.code);
		stage.pName = info.entry.c_str();
		stage.stage = info.stage;
		stage.pNext = VK_NULL_HANDLE;
	}

	// set vertex buffer attributes, so GPU can map values to buffer

	// get the vertex attribute description data
	std::vector<VkVertexInputBindingDescription> bindingDescriptions = builder.vertexBindings;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = builder.vertexAttributes;

	// define how vertex data is passed in
	VkPipelineVertexInputStateCreateInfo vertexInputInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size()),
		.pVertexBindingDescriptions = bindingDescriptions.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
		.pVertexAttributeDescriptions = attributeDescriptions.data(),
	};

	// define how triangle data used
	VkPipelineInputAssemblyStateCreateInfo inputAssembly
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	// define dynamic states for viewport and scissor
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	// create dynamic state info
	VkPipelineDynamicStateCreateInfo dynamicState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data()
	};

	// create viewport state create info (only 1, for now)
	VkPipelineViewportStateCreateInfo viewportState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.scissorCount = 1,
	};

	// create rasterizer info
	VkPipelineRasterizationStateCreateInfo rasterizer
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE, // if true, near and var fragments are clamped instead of discarded
		.rasterizerDiscardEnable = VK_FALSE, // if true, disables geometry shader, so nothing goes through the framebuffer
		.polygonMode = builder.polygonMode, // fill polygon area with fragments (can also do line or point mode for frames/points)
		.cullMode = builder.cullMode, // cull back faces
		.frontFace = builder.frontFace, // how to determine front face/direction to rotate around on triangle vertices
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f, // Optional
		.depthBiasClamp = 0.0f, // Optional
		.depthBiasSlopeFactor = 0.0f, // Optional
		.lineWidth = builder.lineWidth, // use with anything other than fill
	};

	// create multisampling info
	VkPipelineMultisampleStateCreateInfo multisampling
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = 1.0f, // Optional
		.pSampleMask = nullptr, // Optional
		.alphaToCoverageEnable = VK_FALSE, // Optional
		.alphaToOneEnable = VK_FALSE, // Optional
	};

	// create depth stencil info
	VkPipelineDepthStencilStateCreateInfo depthStencil
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
		.front = {},
		.back = {},
		.minDepthBounds = 0.0f,
		.maxDepthBounds = 1.0f,
	};

	// create color blend info, based on alpha
	VkPipelineColorBlendAttachmentState colorBlendAttachment
	{
		.blendEnable = VK_TRUE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
		.alphaBlendOp = VK_BLEND_OP_ADD, // Optional
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};

	// more color blending logic
	VkPipelineColorBlendStateCreateInfo colorBlending
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY, // Optional
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
	};

	//colorBlending.blendConstants[0] = 0.0f; // Optional
	//colorBlending.blendConstants[1] = 0.0f; // Optional
	//colorBlending.blendConstants[2] = 0.0f; // Optional
	//colorBlending.blendConstants[3] = 0.0f; // Optional

	//// push constants
	//VkPushConstantRange pushConstantRange =
	//{
	//	.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
	//	.offset = 0,
	//	.size = sizeof(MeshInfo),
	//};

	// get layout from shader

	// compile all of the information to create the pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = static_cast<uint32_t>(shaderStages.size()),
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		.layout = _shader->get_pipeline_layout(),
		.renderPass = renderer.get_render_pass(),
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional, derive from another existing pipeline
		.basePipelineIndex = -1, // Optional
	};

	// create the pipeline
	VK_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline), "Failed to create graphics pipeline.");

	// cleanup
	for (size_t i = 0; i < shaderStages.size(); i++)
	{
		vkDestroyShaderModule(device, shaderStages.at(i).module, nullptr);
	}
}
