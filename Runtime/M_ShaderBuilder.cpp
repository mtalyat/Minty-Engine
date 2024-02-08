#include "pch.h"
#include "M_ShaderBuilder.h"

#include "M_DrawCallObjectInfo.h"
#include "M_RenderEngine.h"

#include "M_Console.h"

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
