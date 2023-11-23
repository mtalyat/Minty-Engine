#include "pch.h"
#include "M_Rendering_PushConstantInfo.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::PushConstantInfo::PushConstantInfo()
	: _name()
	, _stageFlags()
	, _offset()
	, _size()
{}

minty::rendering::PushConstantInfo::PushConstantInfo(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const offset, uint32_t const size)
	: _name(name)
	, _stageFlags(stageFlags)
	, _offset(offset)
	, _size(size)
{}

std::string const& minty::rendering::PushConstantInfo::get_name() const
{
	return _name;
}

VkShaderStageFlags minty::rendering::PushConstantInfo::get_stage_flags() const
{
	return _stageFlags;
}

uint32_t minty::rendering::PushConstantInfo::get_offset() const
{
	return _offset;
}

uint32_t minty::rendering::PushConstantInfo::get_size() const
{
	return _size;
}

VkPushConstantRange minty::rendering::PushConstantInfo::to_range() const
{
	return VkPushConstantRange
	{
		.stageFlags = _stageFlags,
		.offset = _offset,
		.size = _size
	};
}
