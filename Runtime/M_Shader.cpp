#include "pch.h"
#include "M_Shader.h"

#include "M_Renderer.h"

#include "M_Math.h"
#include "M_File.h"
#include <fstream>

using namespace minty;
using namespace minty::rendering;

minty::Shader::Shader(rendering::ShaderBuilder const& builder, Renderer& renderer)
	: RendererObject::RendererObject(renderer)
	, _descriptorSet(renderer)
{
	for (auto const& pair : builder.pushConstantInfos)
	{
		_pushConstantInfos.emplace(pair.first, pair.second);
	}

	for (auto const& pair : builder.uniformConstantInfos)
	{
		_uniformConstantInfos.emplace(pair.first, pair.second);
	}

	// create layouts
	create_descriptor_set_layouts(builder);
	create_pipeline_layout(builder);

	// create global descriptor set
	_descriptorSet = create_descriptor_set(DESCRIPTOR_SET_SHADER);
}

void minty::Shader::destroy()
{
	auto device = _renderer.get_device();

	_descriptorSet.destroy();
	for (size_t i = 0; i < _descriptorSetLayouts.size(); i++)
	{
		vkDestroyDescriptorSetLayout(device, _descriptorSetLayouts[i], nullptr);
	}
	vkDestroyPipelineLayout(device, _pipelineLayout, nullptr);
	for (auto const& poolSet : _descriptorPools)
	{
		for (auto& pool : poolSet.second)
		{
			vkDestroyDescriptorPool(device, pool.first, nullptr);
		}
	}
	_pushConstantInfos.clear();
	_uniformConstantInfos.clear();
}

VkPipelineLayout minty::Shader::get_pipeline_layout() const
{
	return _pipelineLayout;
}

rendering::DescriptorSet const& minty::Shader::get_global_descriptor_set() const
{
	return _descriptorSet;
}

//void minty::Shader::bind(VkCommandBuffer const commandBuffer) const
//{
//	// bind descriptors
//	//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[_renderer.get_frame()], 0, nullptr);
//}

void minty::Shader::update_push_constant(VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset) const
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(0);

	// push value
	vkCmdPushConstants(commandBuffer, _pipelineLayout, info.stageFlags, info.offset + offset, size, value);
}

void minty::Shader::update_push_constant(std::string const& name, VkCommandBuffer const commandBuffer, void* const value, uint32_t const size, uint32_t const offset) const
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(name);

	// push value
	vkCmdPushConstants(commandBuffer, _pipelineLayout, info.stageFlags, info.offset + offset, size, value);
}

void minty::Shader::update_global_uniform_constant(std::string const& name, void* const value, uint32_t const size, uint32_t const offset) const
{
	_descriptorSet.set(name, value, offset, size);
}

void minty::Shader::create_descriptor_set_layouts(rendering::ShaderBuilder const& builder)
{
	VkDevice device = _renderer.get_device();

	for (int i = 0; i < DESCRIPTOR_SET_COUNT; i++)
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

void minty::Shader::create_pipeline_layout(rendering::ShaderBuilder const& builder)
{
	// get push constant infos
	auto const& pushConstantInfos = builder.pushConstantInfos;

	// convert to vulkan push constants
	std::vector<VkPushConstantRange> pushConstants;
	pushConstants.reserve(pushConstantInfos.size());

	for (auto const& info : pushConstantInfos)
	{
		pushConstants.push_back(VkPushConstantRange{
			.stageFlags = info.second.stageFlags,
			.offset = info.second.offset,
			.size = info.second.size
			});
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
	if (vkCreatePipelineLayout(_renderer.get_device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		error::abort("Failed to create pipeline layout.");
	}
}

VkDescriptorPool minty::Shader::create_pool(uint32_t const set)
{
	uint32_t const maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * DESCRIPTOR_SETS_PER_POOL);

	std::vector<VkDescriptorPoolSize> poolSizes;

	for (auto const& info : _uniformConstantInfos)
	{
		if (info.second.set != set)
		{
			// skip other sets, only get this set
			continue;
		}

		// add pool size, account for all the frames in the flight
		poolSizes.push_back(VkDescriptorPoolSize{
					.type = info.second.type,
					.descriptorCount = info.second.count * maxSets
			});
	}

	VkDescriptorPool descriptorPool;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	if (vkCreateDescriptorPool(_renderer.get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		error::abort("Failed to create descriptor pool.");
	}

	return descriptorPool;
}

VkDescriptorPool minty::Shader::get_pool(uint32_t const set, uint32_t const amount)
{
	uint32_t const maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * DESCRIPTOR_SETS_PER_POOL);

	// find existing, non-full pool for this set
	auto found = _descriptorPools.find(set);

	if (found != _descriptorPools.end())
	{
		// set list found

		if (found->second.size())
		{
			for (auto& pair : found->second)
			{
				// check if pool is not full
				if (pair.second + amount <= maxSets)
				{
					// pool is not full, return it for use

					// inc first
					pair.second += amount;

					return pair.first;
				}
			}
		}
	}
	else
	{
		// no set list found

		// create one
		_descriptorPools.emplace(set, std::vector<std::pair<VkDescriptorPool, uint32_t>>());
	}

	// no set found, so make a new one
	VkDescriptorPool pool = create_pool(set);

	// add for later usage
	_descriptorPools.at(set).push_back(
		{
			pool,
			amount, // set as 1 for first usage
		});

	// return it
	return pool;
}

std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> minty::Shader::create_descriptor_sets(uint32_t const set)
{
	// get a pool for the set
	VkDescriptorPool descriptorPool = get_pool(set);

	// get the layout to use
	VkDescriptorSetLayout descriptorSetLayout = _descriptorSetLayouts.at(set);

	// the set to allocate and write to
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets;

	// allocate descriptor set
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSets.size());
	allocInfo.pSetLayouts = layouts.data();

	// create the descriptor sets
	VK_ASSERT(vkAllocateDescriptorSets(_renderer.get_device(), &allocInfo, descriptorSets.data()), std::format("Failed to allocate descriptor sets for descriptor set layout {}.", set));

	return descriptorSets;
}

DescriptorSet minty::Shader::create_descriptor_set(uint32_t const set)
{
	// create and allocate sets
	auto descriptorSets = create_descriptor_sets(set);

	// get constant infos
	auto const& constantInfos = get_uniform_constant_infos(set);

	// update descriptor set with data

	std::vector<VkWriteDescriptorSet> descriptorWrites;
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::unordered_map<std::string, std::array<ID, MAX_FRAMES_IN_FLIGHT>> buffers;
	std::vector<std::vector<VkDescriptorImageInfo>> imageInfos;

	descriptorWrites.resize(constantInfos.size());

	int setIndex = 0;

	for (auto const set : descriptorSets)
	{
		int i = 0;
		// iterate through Vulkan descriptor sets, set to default Material template values
		for (auto const& info : constantInfos)
		{
			auto const type = info.type;

			switch (type)
			{
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				// create a buffer
				ID bufferId = _renderer.create_buffer_uniform(static_cast<VkDeviceSize>(info.size));

				// add to buffers
				auto const& found = buffers.find(info.name);
				if (found == buffers.end())
				{
					buffers.emplace(info.name, std::array<ID, MAX_FRAMES_IN_FLIGHT>());
				}
				buffers.at(info.name)[setIndex] = bufferId;

				// add buffer info
				bufferInfos.push_back(VkDescriptorBufferInfo());

				// set buffer info
				VkDescriptorBufferInfo& bufferInfo = bufferInfos.back();
				bufferInfo.buffer = _renderer.get_buffer(bufferId);
				bufferInfo.offset = 0;
				bufferInfo.range = _renderer.get_buffer_size(bufferId);

				// create write
				descriptorWrites[i] = {
						.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext = nullptr,
						.dstSet = set,
						.dstBinding = info.binding,
						.dstArrayElement = 0,
						.descriptorCount = info.count,
						.descriptorType = type,
						.pImageInfo = nullptr,
						.pBufferInfo = &bufferInfo,
						.pTexelBufferView = nullptr,
				};

				break;
			}
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				// create new array of image infos
				imageInfos.push_back(std::vector<VkDescriptorImageInfo>());
				auto& list = imageInfos.back();
				auto const& imageIds = info.ids;
				list.resize(imageIds.size());

				// populate with images based on ids
				for (size_t k = 0; k < imageIds.size(); k++)
				{
					ID textureId = imageIds.at(k);

					Texture const& texture = _renderer.get_texture(textureId);

					VkDescriptorImageInfo& imageInfo = list.at(k);

					imageInfo.sampler = texture.get_sampler();
					imageInfo.imageView = texture.get_image_view();
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				}

				// create write
				descriptorWrites[i] = {
						.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext = nullptr,
						.dstSet = set,
						.dstBinding = info.binding,
						.dstArrayElement = 0,
						.descriptorCount = info.count,
						.descriptorType = type,
						.pImageInfo = list.data(),
						.pBufferInfo = nullptr,
						.pTexelBufferView = nullptr,
				};
				break;
			}

			i++;
		}

		vkUpdateDescriptorSets(_renderer.get_device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		setIndex++;
	}

	// all done
	return DescriptorSet(descriptorSets, buffers, _renderer);
}

std::vector<rendering::UniformConstantInfo> minty::Shader::get_uniform_constant_infos(uint32_t const set)
{
	std::vector<rendering::UniformConstantInfo> infos;

	for (auto const& pair : _uniformConstantInfos)
	{
		if (pair.second.set == set)
		{
			infos.push_back(pair.second);
		}
	}

	return infos;
}

std::string minty::to_string(Shader const& shader)
{
	return std::format("Shader()");
}
