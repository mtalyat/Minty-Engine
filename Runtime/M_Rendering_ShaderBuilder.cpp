#include "pch.h"
#include "M_Rendering_ShaderBuilder.h"

#include "M_Rendering_DrawCallObjectInfo.h"
#include "M_Renderer.h"

#include "M_Console.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::ShaderBuilder::ShaderBuilder()
{}

std::unordered_map<std::string, PushConstantInfo> const& minty::rendering::ShaderBuilder::get_push_constant_infos() const
{
	return _pushConstantInfos;
}

std::unordered_map<std::string, UniformConstantInfo> const& minty::rendering::ShaderBuilder::get_uniform_constant_infos() const
{
	return _uniformConstantInfos;
}

std::vector<VkDescriptorSetLayoutBinding> ShaderBuilder::get_descriptor_set_layout_bindings(uint32_t const set) const
{
	std::vector<VkDescriptorSetLayoutBinding> result;

	for (auto const& constant : _uniformConstantInfos)
	{
		if (constant.second.get_set() != set)
		{
			continue;
		}

		result.push_back(VkDescriptorSetLayoutBinding
			{
				.binding = constant.second.get_binding(),
				.descriptorType = constant.second.get_type(),
				.descriptorCount = constant.second.get_count(),
				.stageFlags = constant.second.get_stage_flags(),
				.pImmutableSamplers = nullptr
			});
	}

	return result;
}

std::vector<UniformConstantInfo> minty::rendering::ShaderBuilder::get_uniform_constant_infos(uint32_t const set) const
{
	std::vector<UniformConstantInfo> results;

	for (auto const& constant : _uniformConstantInfos)
	{
		if (constant.second.get_set() == set)
		{
			results.push_back(constant.second);
		}
	}

	return results;
}

uint32_t minty::rendering::ShaderBuilder::get_uniform_constant_count() const
{
	return static_cast<uint32_t>(_uniformConstantInfos.size());
}

uint32_t minty::rendering::ShaderBuilder::get_uniform_constant_count(uint32_t const set) const
{
	uint32_t count = 0;

	for (auto const& constant : _uniformConstantInfos)
	{
		if (constant.second.get_set() == set)
		{
			count++;
		}
	}

	return count;
}

void minty::rendering::ShaderBuilder::emplace_push_constant(PushConstantInfo const& info)
{
	_pushConstantInfos[info.get_name()] = info;
}

void minty::rendering::ShaderBuilder::emplace_uniform_constant(UniformConstantInfo const& info)
{
	_uniformConstantInfos[info.get_name()] = info;
}

std::string minty::rendering::to_string(ShaderBuilder const& value)
{
	return std::format("ShaderBuilder()");
}
