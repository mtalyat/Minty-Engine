#include "pch.h"
#include "M_Shader.h"

#include "M_RenderEngine.h"

#include "M_Math.h"
#include "M_File.h"
#include <fstream>

using namespace minty;
using namespace minty::rendering;

minty::Shader::Shader()
	: RenderObject::RenderObject()
	, _descriptorSet()
{}

minty::Shader::Shader(rendering::ShaderBuilder const& builder, RenderEngine& renderer)
	: RenderObject::RenderObject(&renderer)
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
	_descriptorSet = create_descriptor_set(DESCRIPTOR_SET_SHADER, true);
}

void minty::Shader::destroy()
{
	RenderEngine* renderer = get_renderer();

	auto device = renderer->get_device();

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

void minty::Shader::update_push_constant(VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset)
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(0);

	// push value
	vkCmdPushConstants(commandBuffer, _pipelineLayout, info.stageFlags, info.offset + offset, size, value);
}

void minty::Shader::update_push_constant(String const& name, VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset)
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(name);

	// push value
	vkCmdPushConstants(commandBuffer, _pipelineLayout, info.stageFlags, info.offset + offset, size, value);
}

void minty::Shader::update_global_uniform_constant(String const& name, void const* const value, uint32_t const size, uint32_t const offset)
{
	_descriptorSet.set(name, value, size, offset);
	_descriptorSet.apply();
}

void minty::Shader::update_global_uniform_constant(String const& name, int const frame, void const* const value, uint32_t const size, uint32_t const offset)
{
	_descriptorSet.set(name, frame, value, size, offset);
	_descriptorSet.apply(frame);
}

void minty::Shader::create_descriptor_set_layouts(rendering::ShaderBuilder const& builder)
{
	RenderEngine* renderer = get_renderer();

	VkDevice device = renderer->get_device();

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
			.stageFlags = static_cast<VkShaderStageFlags>(info.second.stageFlags),
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

	RenderEngine* renderer = get_renderer();

	// create the pipeline layout
	if (vkCreatePipelineLayout(renderer->get_device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
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

	if (vkCreateDescriptorPool(get_renderer()->get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
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
	VK_ASSERT(vkAllocateDescriptorSets(get_renderer()->get_device(), &allocInfo, descriptorSets.data()), std::format("Failed to allocate descriptor sets for descriptor set layout {}.", set));

	return descriptorSets;
}

DescriptorSet minty::Shader::create_descriptor_set(uint32_t const set, bool const initialize)
{
	RenderEngine* renderer = get_renderer();

	if (set == DESCRIPTOR_SET_INVALID)
	{
		// invalid set, so just create an empty descriptor
		return DescriptorSet(*renderer);
	}

	// create and allocate sets
	auto descriptorSets = create_descriptor_sets(set);

	// get constant infos
	auto const& constantInfos = get_uniform_constant_infos(set);

	// update descriptor set with data
	std::unordered_map<String, std::array<DescriptorSet::DescriptorData, MAX_FRAMES_IN_FLIGHT>> datas;

	int frame = 0;

	for (auto const set : descriptorSets)
	{
		int i = 0;

		// iterate through Vulkan descriptor sets, set to default Material template values
		for (auto const& info : constantInfos)
		{
			auto const type = info.type;

			// create a new data for this constant, if needed
			auto const& found = datas.find(info.name);
			if (found == datas.end())
			{
				// add new array
				datas.emplace(info.name, std::array<DescriptorSet::DescriptorData, MAX_FRAMES_IN_FLIGHT>());
			}

			// get the data we are using
			auto& data = datas.at(info.name).at(frame);

			// set the shared data for each write
			data.empty = true;
			data.type = type;
			data.binding = info.binding;
			data.count = info.count;
			data.ids.clear();

			switch (type)
			{
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				// resize ids to one
				data.ids.resize(1);

				// create a buffer
				ID bufferId = renderer->create_buffer_uniform(static_cast<VkDeviceSize>(info.size));

				// add to buffers
				data.ids[0] = bufferId;

				// not empty by default
				data.empty = false;

				break;
			}
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			{
				// resize image infos
				data.ids.resize(static_cast<size_t>(info.count));

				// empty by default, until ids are populated

				break;
			}
			}

			i++;
		}

		frame++;
	}

	// all done, create set
	DescriptorSet descriptorSet(descriptorSets, datas, *renderer);

	// "initialize" it if told to
	if (initialize)
	{
		descriptorSet.apply();
	}

	return descriptorSet;
}

std::vector<rendering::UniformConstantInfo> minty::Shader::get_uniform_constant_infos(uint32_t const set) const
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

String minty::to_string(Shader const& shader)
{
	return std::format("Shader()");
}
