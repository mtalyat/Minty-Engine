#include "pch.h"
#include "M_Rendering_UniformConstantInfo.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::UniformConstantInfo::UniformConstantInfo()
	: _name()
	, _type()
	, _set()
	, _binding()
	, _count()
	, _size()
	, _stageFlags()
	, _ids()
{}

minty::rendering::UniformConstantInfo::UniformConstantInfo(std::string const& name, VkShaderStageFlags const stageFlags, VkDescriptorType const type, VkDeviceSize const size, uint32_t const set, uint32_t const binding, uint32_t const count, std::vector<ID> const& ids)
	: _name(name)
	, _type(type)
	, _set(set)
	, _binding(binding)
	, _count(count)
	, _size(size)
	, _stageFlags(stageFlags)
	, _ids(ids)
{}

std::string const& minty::rendering::UniformConstantInfo::get_name() const
{
	return _name;
}

VkDescriptorType minty::rendering::UniformConstantInfo::get_type() const
{
	return _type;
}

uint32_t minty::rendering::UniformConstantInfo::get_set() const
{
	return _set;
}

uint32_t minty::rendering::UniformConstantInfo::get_binding() const
{
	return _binding;
}

uint32_t minty::rendering::UniformConstantInfo::get_count() const
{
	return _count;
}

VkDeviceSize minty::rendering::UniformConstantInfo::get_size() const
{
	return _size;
}

VkShaderStageFlags minty::rendering::UniformConstantInfo::get_stage_flags() const
{
	return _stageFlags;
}

std::vector<ID> const& minty::rendering::UniformConstantInfo::get_ids() const
{
	return _ids;
}
