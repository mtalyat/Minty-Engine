#include "pch.h"
#include "M_Shader.h"

#include "M_Renderer.h"
#include "M_Rendering_ShaderBuilder.h"

#include "M_Math.h"
#include "M_File.h"
#include <fstream>

using namespace minty;
using namespace minty::rendering;

//void Renderer::create_uniform_buffers()
//{
//	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
//
//	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
//	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
//
//	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//		create_buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
//
//		vkMapMemory(_device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
//	}
//}

//void minty::Renderer::create_material_buffers()
//{
//	// allocate space for materials
//	VkDeviceSize bufferSize = sizeof(MaterialInfo) * MAX_MATERIALS;
//
//	materialBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//	materialBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
//	materialBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
//
//	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//		create_buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, materialBuffers[i], materialBuffersMemory[i]);
//
//		vkMapMemory(_device, materialBuffersMemory[i], 0, bufferSize, 0, &materialBuffersMapped[i]);
//	}
//}

minty::Shader::Shader(std::string const& vertexPath, std::string const& fragmentPath, rendering::ShaderBuilder const& builder, Renderer& renderer)
	: RendererObject::RendererObject(renderer)
{
	_uniformCount = builder.get_uniform_constant_count(0);
	_materialSize = builder.get_material_size();

	// first, create descriptor layouts
	create_descriptor_set_layouts(builder);

	// create buffers to store descriptor data
	create_buffers(builder);

	// create shader modules/shader itself
	create_shader(vertexPath, fragmentPath, builder);

	// create descriptor pool and the descriptor sets
	create_descriptor_pool(builder);
	create_descriptor_sets(builder);
}

void minty::Shader::destroy()
{
	auto device = _renderer.get_device();

	for (size_t i = 0; i < _buffers.size(); i++) {
		vkDestroyBuffer(device, _buffers[i], nullptr);
		vkFreeMemory(device, _memories[i], nullptr);
	}
	vkDestroyDescriptorPool(device, _descriptorPool, nullptr);
	for (size_t i = 0; i < _descriptorSetLayouts.size(); i++)
	{
		vkDestroyDescriptorSetLayout(device, _descriptorSetLayouts[i], nullptr);
	}
	vkDestroyPipeline(device, _pipeline, nullptr);
	vkDestroyPipelineLayout(device, _layout, nullptr);
}

size_t minty::Shader::get_material_size() const
{
	return _materialSize;
}

void minty::Shader::bind(VkCommandBuffer const commandBuffer) const
{
	// bind descriptors
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _layout, 0, 1, &_descriptorSets[_renderer.get_frame()], 0, nullptr);

	// bind pipeline
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
}

void minty::Shader::update_push_constant(VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t offset) const
{
	// get info of push constant
	auto const& info = _pushConstants.at(0);

	// push value
	vkCmdPushConstants(commandBuffer, _layout, info.flags, offset, size, value);
}

void minty::Shader::update_push_constant(std::string const& name, VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t offset) const
{
	// get info of push constant
	auto const& info = _pushConstants.at(name);

	// push value
	vkCmdPushConstants(commandBuffer, _layout, info.flags, offset, size, value);
}

void minty::Shader::update_uniform_constant(std::string const& name, void const* const value, size_t const elementSize, size_t const count, size_t const index) const
{
	auto id = _uniformConstants.get_id(name);

	MINTY_ASSERT(id >= 0, std::format("Uniform constant \"{}\" not found.", name));

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		size_t bufferIndex = get_buffer_index(id, i);

		byte* ptr = static_cast<byte*>(_mapped.at(bufferIndex));

		memcpy(ptr + index * elementSize, value, elementSize * count);
	}
}

void minty::Shader::update_uniform_constant_frame(std::string const& name, void const* const value, size_t const elementSize, size_t const count, size_t const index) const
{
	auto id = _uniformConstants.get_id(name);

	MINTY_ASSERT(id >= 0, std::format("Uniform constant \"{}\" not found.", name));

	size_t bufferIndex = get_buffer_index(id, _renderer.get_frame());

	byte* ptr = static_cast<byte*>(_mapped.at(bufferIndex));

	memcpy(ptr + index * elementSize, value, elementSize * count);
}

size_t minty::Shader::get_buffer_index(size_t const buffer, size_t const frame) const
{
	return frame * _uniformCount + buffer;
}

size_t minty::Shader::get_buffer_count() const
{
	return _uniformCount * MAX_FRAMES_IN_FLIGHT;
}

std::vector<char> read_file_raw(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule minty::Shader::load_module(std::string const& path)
{
	std::vector<char> code = read_file_raw(path);

	VkShaderModuleCreateInfo createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_renderer.get_device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		error::abort("failed to create shader module!");
	}

	return shaderModule;
}

void minty::Shader::create_descriptor_set_layouts(rendering::ShaderBuilder const& builder)
{
	//// UniformBufferObject
	//VkDescriptorSetLayoutBinding uboLayoutBinding{};
	//uboLayoutBinding.binding = 0;
	//uboLayoutBinding.descriptorCount = 1;
	//uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	//// MaterialsBufferObject
	//VkDescriptorSetLayoutBinding mboLayoutBinding{};
	//mboLayoutBinding.binding = 1;
	//mboLayoutBinding.descriptorCount = 1;
	//mboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//mboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	//// texSamplers[]
	//VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	//samplerLayoutBinding.binding = 2;
	//samplerLayoutBinding.descriptorCount = static_cast<uint32_t>(MAX_TEXTURES);
	//samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//samplerLayoutBinding.pImmutableSamplers = nullptr;
	//samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	//std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, mboLayoutBinding, samplerLayoutBinding };

	//VkDescriptorSetLayoutCreateInfo layoutInfo{};
	//layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	//layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	//layoutInfo.pBindings = bindings.data();

	size_t count = builder.get_descriptor_set_layout_count();
	_descriptorSetLayouts.resize(count);

	VkDevice device = _renderer.get_device();

	for (size_t i = 0; i < count; i++)
	{
		auto layoutBindings = builder.get_descriptor_set_layout_bindings(static_cast<uint32_t>(i));

		VkDescriptorSetLayoutCreateInfo layoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = static_cast<uint32_t>(layoutBindings.size()),
			.pBindings = layoutBindings.data(),
		};

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &_descriptorSetLayouts.at(i)) != VK_SUCCESS) {
			error::abort("Failed to create descriptor set layout.");
		}
	}
}

void minty::Shader::create_shader(std::string const& vertexPath, std::string const& fragmentPath, rendering::ShaderBuilder const& builder)
{
	uint32_t i;

	// create 
	VkDevice device = _renderer.get_device();

	// load shaders from disk
	VkShaderModule vertShaderModule = load_module(vertexPath);
	VkShaderModule fragShaderModule = load_module(fragmentPath);

	// create info for vertex shader
	VkPipelineShaderStageCreateInfo vertShaderStageInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertShaderModule,
		.pName = builder.get_vertex_enter_point().data(),
	};

	// create info for fragment shader
	VkPipelineShaderStageCreateInfo fragShaderStageInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragShaderModule,
		.pName = builder.get_fragment_enter_point().data(),
	};

	// stick into one array
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// set vertex buffer attributes, so GPU can map values to buffer

	// get the vertex attribute description data
	std::vector<VkVertexInputBindingDescription> bindingDescriptions = builder.get_vertex_bindings();
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = builder.get_vertex_attributes();

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
		.polygonMode = builder.get_polygon_mode(), // fill polygon area with fragments (can also do line or point mode for frames/points)
		.cullMode = builder.get_cull_mode(), // cull back faces
		.frontFace = builder.get_front_face(), // how to determine front face/direction to rotate around on triangle vertices
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f, // Optional
		.depthBiasClamp = 0.0f, // Optional
		.depthBiasSlopeFactor = 0.0f, // Optional
		.lineWidth = builder.get_line_width(), // use with anything other than fill
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
		.blendEnable = VK_FALSE,
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

	auto const& pushConstantInfos = builder.get_push_constant_infos();

	std::vector<VkPushConstantRange> pushConstants;
	pushConstants.reserve(pushConstantInfos.size());
	_pushConstants.reserve(static_cast<ID>(pushConstantInfos.size()));

	i = 0;
	for (auto const& info : pushConstantInfos)
	{
		pushConstants.push_back(info.range);
		_pushConstants.emplace(info.name, PushConstantInfo
			{
				.flags = info.range.stageFlags
			});
		i++;
	}

	// create the layout of the pipeline
	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = static_cast<uint32_t>(_descriptorSetLayouts.size()),
		.pSetLayouts = _descriptorSetLayouts.data(),
		.pushConstantRangeCount = static_cast<uint32_t>(pushConstants.size()),
		.pPushConstantRanges = pushConstants.data(),
	};

	// create the pipeline layout
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_layout) != VK_SUCCESS) {
		error::abort("Failed to create pipeline layout.");
	}

	// compile all of the information to create the pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pStages = shaderStages,
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		.layout = _layout,
		.renderPass = _renderer.get_render_pass(),
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional, derive from another existing pipeline
		.basePipelineIndex = -1, // Optional
	};

	// create the pipeline
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS) {
		error::abort("Failed to create graphics pipeline.");
	}

	// cleanup shader modules
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void minty::Shader::create_buffers(rendering::ShaderBuilder const& builder)
{
	// get count
	auto count = get_buffer_count();

	// resize all buffers
	_buffers.resize(count);
	_memories.resize(count);
	_mapped.resize(count);

	// get info about constants
	auto const& constantInfos = builder.get_uniform_constant_infos(0);

	uint32_t j;
	size_t index;
	VkDevice device = _renderer.get_device();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		j = 0;
		for (auto const& info : constantInfos)
		{
			index = get_buffer_index(j, i);

			// create the buffer
			_renderer.create_buffer(info.size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffers[index], _memories[index]);

			// map the memory so the values can be set
			VK_ASSERT(result, vkMapMemory(device, _memories[index], 0, info.size, 0, &_mapped[index]), "Failed to map memory for new buffer.");

			j++;
		}
	}

	// register constants so their values can be set later
	for (auto const& info : constantInfos)
	{
		_uniformConstants.emplace(info.name, UniformConstantInfo
			{
				// nothing yet
			});
	}
}

void minty::Shader::create_descriptor_pool(rendering::ShaderBuilder const& builder)
{
	//VkDescriptorPoolSize poolSize{};
	//poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	auto constants = builder.get_uniform_constant_infos(0);

	uint32_t maxSets = static_cast<uint32_t>(_descriptorSetLayouts.size() * MAX_FRAMES_IN_FLIGHT);

	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(constants.size());

	for (size_t i = 0; i < constants.size(); i++)
	{
		poolSizes[i] = {
				.type = constants[i].type,
				.descriptorCount = MAX_FRAMES_IN_FLIGHT * constants[i].count, // account for each frame in the flight
		};
	}

	//std::array<VkDescriptorPoolSize, 3> poolSizes{};
	//poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); // UBO
	//poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * MAX_MATERIALS); // MBO
	//poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * MAX_TEXTURES); // samplers

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	if (vkCreateDescriptorPool(_renderer.get_device(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		error::abort("Failed to create descriptor pool.");
	}
}

void minty::Shader::create_descriptor_sets(rendering::ShaderBuilder const& builder)
{
	// allocate descriptor sets
	_descriptorSets.resize(_descriptorSetLayouts.size() * MAX_FRAMES_IN_FLIGHT);
	size_t i = 0;
	for (auto& descriptorSetLayout : _descriptorSetLayouts)
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
		allocInfo.pSetLayouts = layouts.data();

		VK_ASSERT(result, vkAllocateDescriptorSets(_renderer.get_device(), &allocInfo, &_descriptorSets[i]), std::format("Failed to allocate descriptor sets for descriptor set layout {}.", i));
		i++;
	}

	auto const& constantInfos = builder.get_uniform_constant_infos(0);

	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::vector<std::vector<VkDescriptorImageInfo>> imageInfos;
	std::vector<VkWriteDescriptorSet> descriptorWrites;

	// write descriptor sets
	for (i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		// keep track of infos so they can be properly disposed of after creation
		bufferInfos.clear();
		bufferInfos.resize(constantInfos.size());
		imageInfos.clear();

		// create write structs
		descriptorWrites.clear();
		descriptorWrites.resize(constantInfos.size());

		int j = 0;
		for (auto const& info : constantInfos)
		{
			size_t index = get_buffer_index(j, i);

			switch (info.type)
			{
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				// set buffer info
				VkDescriptorBufferInfo* bufferInfo = &bufferInfos.at(j);

				bufferInfo->buffer = _buffers.at(index);
				bufferInfo->offset = 0;
				bufferInfo->range = info.size;

				// create write
				descriptorWrites[j] = {
						.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext = nullptr,
						.dstSet = _descriptorSets.at(i),
						.dstBinding = info.binding,
						.dstArrayElement = 0,
						.descriptorCount = info.count,
						.descriptorType = info.type,
						.pImageInfo = nullptr,
						.pBufferInfo = bufferInfo,
						.pTexelBufferView = nullptr,
				};

				bufferInfo = nullptr;

				//console::log(std::format("Buffer: {}, offset: {}, range: {}", reinterpret_cast<size_t>(&bufferInfos.back().buffer), bufferInfos.back().offset, bufferInfos.back().range));
				//console::log(std::format("Buffer: {}, offset: {}, range: {}", reinterpret_cast<size_t>(&descriptorWrites[j].pBufferInfo->buffer), descriptorWrites[j].pBufferInfo->offset, descriptorWrites[j].pBufferInfo->range));
				break;
			}
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				// create new array of image infos
				imageInfos.push_back(std::vector<VkDescriptorImageInfo>());
				auto& list = imageInfos.back();
				list.reserve(info.count);

				// populate with images based on ids
				ID textureCount = _renderer.get_texture_count();
				for (ID k = 0; k < textureCount; k++)
				{
					Texture const& texture = _renderer.get_texture(k);
					list.push_back(VkDescriptorImageInfo
						{
							.sampler = texture.get_sampler(),
							.imageView = texture.get_image_view(),
							.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
						});
				}

				// create write
				descriptorWrites[j] = {
						.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext = nullptr,
						.dstSet = _descriptorSets.at(i),
						.dstBinding = info.binding,
						.dstArrayElement = 0,
						.descriptorCount = info.count,
						.descriptorType = info.type,
						.pImageInfo = list.data(),
						.pBufferInfo = nullptr,
						.pTexelBufferView = nullptr,
				};
				break;
			}
			j++;
		}

		vkUpdateDescriptorSets(_renderer.get_device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		//std::array<VkWriteDescriptorSet, 3> descriptorWrites = {};

		//// UBO
		//VkDescriptorBufferInfo uboBufferInfo = {};
		//uboBufferInfo.buffer = uniformBuffers[i];
		//uboBufferInfo.offset = 0;
		//uboBufferInfo.range = VK_WHOLE_SIZE;

		//descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptorWrites[0].dstSet = descriptorSets[i];
		//descriptorWrites[0].dstBinding = 0;
		//descriptorWrites[0].dstArrayElement = 0;
		//descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//descriptorWrites[0].descriptorCount = 1;
		//descriptorWrites[0].pBufferInfo = &uboBufferInfo;
		//descriptorWrites[0].pImageInfo = 0;

		//// MBO
		//VkDescriptorBufferInfo mboBufferInfo = {};
		//mboBufferInfo.buffer = materialBuffers[i];
		//mboBufferInfo.offset = 0;
		//mboBufferInfo.range = VK_WHOLE_SIZE;

		//descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptorWrites[1].dstSet = descriptorSets[i];
		//descriptorWrites[1].dstBinding = 1;
		//descriptorWrites[1].dstArrayElement = 0;
		//descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//descriptorWrites[1].descriptorCount = 1;
		//descriptorWrites[1].pBufferInfo = &mboBufferInfo;
		//descriptorWrites[1].pImageInfo = 0;

		//// TEX
		//imageInfos.resize(MAX_TEXTURES);
		//for (size_t j = 0; j < MAX_TEXTURES; j++)
		//{
		//	if (j < _textures.size())
		//	{
		//		Texture const& tex = get_texture(static_cast<ID>(j));

		//		imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//		imageInfos[j].imageView = tex._view;
		//		imageInfos[j].sampler = tex._sampler;
		//	}
		//	else
		//	{
		//		// nothing?
		//		//imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		//	}
		//}

		//descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptorWrites[2].dstSet = descriptorSets[i];
		//descriptorWrites[2].dstBinding = 2;
		//descriptorWrites[2].dstArrayElement = 0;
		//descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//descriptorWrites[2].descriptorCount = static_cast<uint32_t>(_textures.size());
		//descriptorWrites[2].pBufferInfo = 0;
		//descriptorWrites[2].pImageInfo = imageInfos.data();
	}
}
