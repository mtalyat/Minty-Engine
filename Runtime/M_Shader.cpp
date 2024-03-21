#include "pch.h"
#include "M_Shader.h"

#include "M_Runtime.h"
#include "M_RenderEngine.h"

#include "M_Math.h"
#include "M_File.h"
#include <fstream>

using namespace minty;
using namespace minty;

std::vector<VkDescriptorSetLayoutBinding> ShaderBuilder::get_descriptor_set_layout_bindings(uint32_t const set) const
{
	std::vector<VkDescriptorSetLayoutBinding> result;

	for (auto const& constant : uniformConstantInfos)
	{
		if (constant.second.set != set)
		{
			continue;
		}

		result.push_back(VkDescriptorSetLayoutBinding
			{
				.binding = constant.second.binding,
				.descriptorType = constant.second.type,
				.descriptorCount = constant.second.count,
				.stageFlags = constant.second.stageFlags,
				.pImmutableSamplers = nullptr
			});
	}

	return result;
}

uint32_t minty::ShaderBuilder::get_uniform_constant_count(uint32_t const set) const
{
	uint32_t count = 0;

	for (auto const& constant : uniformConstantInfos)
	{
		if (constant.second.set == set)
		{
			count++;
		}
	}

	return count;
}

minty::Shader::Shader()
	: Asset()
	, _descriptorSet()
{}

minty::Shader::Shader(ShaderBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _descriptorSet(DescriptorSetBuilder{ .shader = this }, engine)
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

minty::Shader::~Shader()
{
	destroy();
}

void minty::Shader::destroy()
{
	RenderEngine& renderer = get_runtime().get_render_engine();

	VkDevice device = renderer.get_device();

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

DescriptorSet const& minty::Shader::get_global_descriptor_set() const
{
	return _descriptorSet;
}

void minty::Shader::update_push_constant(VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset)
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(0);

	MINTY_ASSERT_FORMAT(size <= info.size, "The given size must not be larger than the push constant info size. push constant: {}, given size: {}, push constant size: {}", info.name, size, info.size);
	MINTY_ASSERT_FORMAT(info.offset + offset + size <= info.size, "The given offset and size will exceed the bounds of the push constant info size. push constant: {}, given offset: {}, given size: {}, push constant offset: {}, push constant size: {}", info.name, offset, size, info.offset, info.size);

	// push value
	vkCmdPushConstants(commandBuffer, _pipelineLayout, info.stageFlags, info.offset + offset, size, value);
}

void minty::Shader::update_push_constant(String const& name, VkCommandBuffer const commandBuffer, void const* const value, uint32_t const size, uint32_t const offset)
{
	// get info of push constant
	auto const& info = _pushConstantInfos.at(name);

	MINTY_ASSERT_FORMAT(size <= info.size, "The given size must not be larger than the push constant info size. push constant: {}, given size: {}, push constant size: {}", info.name, size, info.size);
	MINTY_ASSERT_FORMAT(info.offset + offset + size <= info.size, "The given offset and size will exceed the bounds of the push constant info size. push constant: {}, given offset: {}, given size: {}, push constant offset: {}, push constant size: {}", info.name, offset, size, info.offset, info.size);

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

void minty::Shader::create_descriptor_set_layouts(ShaderBuilder const& builder)
{
	RenderEngine& renderer = get_runtime().get_render_engine();

	VkDevice device = renderer.get_device();

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
			MINTY_ABORT("Failed to create descriptor set layout.");
		}
	}
}

void minty::Shader::create_pipeline_layout(ShaderBuilder const& builder)
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

	RenderEngine& renderer = get_runtime().get_render_engine();

	// create the pipeline layout
	if (vkCreatePipelineLayout(renderer.get_device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		MINTY_ABORT("Failed to create pipeline layout.");
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
	//poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	VK_ASSERT(vkCreateDescriptorPool(get_runtime().get_render_engine().get_device(), &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor pool.");

	return descriptorPool;
}

VkDescriptorPool minty::Shader::take_pool(uint32_t const set, uint32_t const amount)
{
	uint32_t const maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * DESCRIPTOR_SETS_PER_POOL);

	// find existing, non-full pool for this set
	auto found = _descriptorPools.find(set);

	if (found != _descriptorPools.end())
	{
		// set list found

		if (found->second.size())
		{
			for (auto& [pool, count] : found->second)
			{
				// check if pool is not full
				if (count + amount <= maxSets)
				{
					// pool is not full, return it for use

					// inc first
					count += amount;

					return pool;
				}
			}
		}
	}
	else
	{
		// no set list found

		// create one
		_descriptorPools.emplace(set, std::unordered_map<VkDescriptorPool, uint32_t>());
	}

	// no set found, so make a new one
	VkDescriptorPool pool = create_pool(set);

	// add for later usage
	_descriptorPools.at(set).emplace(pool, amount);

	// return it
	return pool;
}

void minty::Shader::give_pool(uint32_t const set, VkDescriptorPool const& pool, uint32_t const amount)
{
	// find the group of pools for this set
	auto found = _descriptorPools.find(set);

	if (found == _descriptorPools.end())
	{
		MINTY_ABORT("Attempting to give pool when there are no pools for the given set.");
	}

	// find the pool itself
	auto found2 = found->second.find(pool);

	if (found2 == found->second.end())
	{
		MINTY_ABORT("Missing descriptor pool for descriptor set within shader.");
	}

	MINTY_ASSERT(found2->second >= amount);

	found2->second -= amount;
}

std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> minty::Shader::create_descriptor_sets(VkDescriptorPool const& pool, uint32_t const set)
{
	// get the layout to use
	VkDescriptorSetLayout descriptorSetLayout = _descriptorSetLayouts.at(set);

	// the set to allocate and write to
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets;

	// allocate descriptor set
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSets.size());
	allocInfo.pSetLayouts = layouts.data();

	// create the descriptor sets
	VK_ASSERT(vkAllocateDescriptorSets(get_runtime().get_render_engine().get_device(), &allocInfo, descriptorSets.data()), std::format("Failed to allocate descriptor sets for descriptor set layout {}.", set));

	return descriptorSets;
}

DescriptorSet minty::Shader::create_descriptor_set(uint32_t const set, bool const initialize)
{
	RenderEngine& renderer = get_runtime().get_render_engine();

	if (set == DESCRIPTOR_SET_INVALID)
	{
		// invalid set, so just create an empty descriptor
		return DescriptorSet(DescriptorSetBuilder
			{
				.shader = this,
			}, get_runtime());
	}

	// create and allocate sets
	
	// get a pool for the set
	DescriptorSetBuilder builder
	{
		.shader = this,
		.pool = take_pool(set),
		.set = set,
	};

	builder.descriptorSets = create_descriptor_sets(builder.pool, set);

	// get constant infos
	auto const& constantInfos = get_uniform_constant_infos(set);

	int frame = 0;

	for (auto const set : builder.descriptorSets)
	{
		int i = 0;

		// iterate through Vulkan descriptor sets, set to default Material template values
		for (auto const& info : constantInfos)
		{
			auto const type = info.type;

			// create a new data for this constant, if needed
			auto found = builder.datas.find(info.name);
			if (found == builder.datas.end())
			{
				// add new array
				builder.datas.emplace(info.name, std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>());
			}

			// get the data we are using
			auto& data = builder.datas.at(info.name).at(frame);

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
				Buffer const& buffer = renderer.create_buffer_uniform(static_cast<VkDeviceSize>(info.size));

				// add to buffers
				data.ids[0] = buffer.get_id();

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
	DescriptorSet descriptorSet(builder, get_runtime());

	// "initialize" it if told to
	if (initialize)
	{
		descriptorSet.apply();
	}

	return descriptorSet;
}

void minty::Shader::free_descriptor_set(DescriptorSet const& set)
{
	MINTY_ABORT("Shader::free_descriptor_set not implemented");
	//MINTY_ASSERT(set._shader == this);

	//RenderEngine& renderer = get_runtime().get_render_engine();

	// take from the count for this pool
	//give_pool(set._set, set._descriptorPool);

	// free the descriptor sets from the pool
	//vkFreeDescriptorSets(renderer.get_device(), set._descriptorPool, static_cast<uint32_t>(set._descriptorSets.size()), set._descriptorSets.data());
}

std::vector<UniformConstantInfo> minty::Shader::get_uniform_constant_infos(uint32_t const set) const
{
	std::vector<UniformConstantInfo> infos;

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
