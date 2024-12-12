#include "pch.h"
#include "VulkanMaterial.h"

#include "Minty/Asset/AssetManager.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanTexture.h"

Minty::VulkanMaterial::VulkanMaterial(MaterialBuilder const& builder)
	: Material(builder)
	, m_frames(MAX_FRAMES_IN_FLIGHT)
{
	Ref<MaterialTemplate> materialTemplate = builder.materialTemplate;
	MINTY_ASSERT(materialTemplate != nullptr);
	Ref<VulkanShader> shader = static_cast<Ref<VulkanShader>>(materialTemplate->get_shader());
	MINTY_ASSERT(shader != nullptr);
	std::vector<ShaderInput> inputs = shader->get_inputs();

	// split up inputs from push constants
	std::vector<Minty::ShaderInput> descriptors;
	descriptors.reserve(inputs.size());
	std::vector<Minty::ShaderInput> pushConstants;
	pushConstants.reserve(inputs.size());

	// save inputs as well
	for (ShaderInput const& input : inputs)
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

	// initialize the frames
	std::unordered_map<VkDescriptorType, uint32_t> descriptorTypeCounts;
	for (ShaderInput const& descriptor : descriptors)
	{
		MINTY_ASSERT_MESSAGE(descriptor.count != 0, "Cannot create a Descriptor with a count of 0.");
		MINTY_ASSERT_MESSAGE(!descriptor.name.empty(), "Cannot create a Descriptor with no name.");

		// create uniform buffers
		if (descriptor.size > 0)
		{
			// create one buffer for each frame in flight
			for (Size j = 0; j < MAX_FRAMES_IN_FLIGHT; j++)
			{
				// create uniform data buffers for each frame
				Frame& frame = m_frames.at(j);
				BufferBuilder bufferBuilder{};
				bufferBuilder.frequent = descriptor.frequent;
				bufferBuilder.size = descriptor.size;
				bufferBuilder.usage = BufferUsage::Uniform;
				bufferBuilder.data = nullptr;
				frame.buffers.push_back(Owner<VulkanBuffer>(bufferBuilder));
			}
		}

		// allocate descriptor sets for each frame
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(MAX_FRAMES_IN_FLIGHT, shader->get_descriptor_set_layout());
		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.descriptorPool = shader->get_descriptor_pool(1);
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

		// write all the inputs to the descriptor set
		for (Frame& frame : m_frames)
		{
			std::vector<VkDescriptorBufferInfo> bufferInfos;
			std::vector<VkDescriptorImageInfo> imageInfos;
			bufferInfos.reserve(descriptors.size());
			imageInfos.reserve(descriptors.size());

			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.reserve(descriptors.size());

			// create buffers
			for (ShaderInput const& descriptor : descriptors)
			{
				switch (descriptor.type)
				{
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
					Ref<VulkanBuffer> vulkanBuffer = frame.buffers.at(shader->get_descriptor_buffer_index(descriptor.name));

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

	// set initial values
	std::unordered_set<String> setValues;

	// set all override values in the material
	DynamicContainer container;
	for (auto const& [name, cargo] : get_values())
	{
		// compile value to one array of byte data
		container.clear();
		cargo.collect(container);

		// set the input
		set_input(name, container.data());

		// mark as set
		setValues.emplace(name);
	}

	// set any remaining not covered values
	for (auto const& [name, cargo] : materialTemplate->get_values())
	{
		// skip if override value was set
		if (setValues.contains(name)) continue;

		// use default value
		container.clear();
		cargo.collect(container);
		set_input(name, container.data());
	}
}

void Minty::VulkanMaterial::on_bind()
{
	VkCommandBuffer commandBuffer = VulkanRenderer::get_command_buffer();
	Frame& frame = m_frames.at(VulkanRenderer::get_current_frame_index());

	Ref<MaterialTemplate> materialTemplate = get_template();
	MINTY_ASSERT(materialTemplate != nullptr);
	Ref<VulkanShader> shader = static_cast<Ref<VulkanShader>>(materialTemplate->get_shader());
	MINTY_ASSERT(shader != nullptr);

	// bind descriptor set(s)
	VulkanRenderer::bind_descriptor_set(commandBuffer, shader->get_layout(), frame.descriptorSet);
}

void Minty::VulkanMaterial::set_input(String const& name, void const* const data)
{
	Ref<MaterialTemplate> materialTemplate = get_template();
	MINTY_ASSERT(materialTemplate != nullptr);
	Ref<VulkanShader> shader = static_cast<Ref<VulkanShader>>(materialTemplate->get_shader());
	MINTY_ASSERT(shader != nullptr);

	MINTY_ASSERT_FORMAT(shader->has_input(name), "\"{}\" is not the name of any constant in this Shader. Shader: \"{}\". Material: \"{}\".", name, AssetManager::get_path(shader->id()).generic_string(), AssetManager::get_path(id()).generic_string());
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot set_input to null.");

	ShaderInput const& input = shader->get_input(name);

	switch (input.type)
	{
	case ShaderInputType::UniformBuffer:
	case ShaderInputType::StorageBuffer:
	{
		// get index of buffer
		Size index = shader->get_descriptor_buffer_index(name);

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
			VulkanTexture const* vulkanTexture = *static_cast<VulkanTexture const* const*>(data);

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
		vkCmdPushConstants(VulkanRenderer::get_command_buffer(), shader->get_layout(), VulkanRenderer::shader_stage_to_vulkan(input.stage), input.offset, input.size, data);
	}
	break;
	}
}