#pragma once

#include "M_Object.h"

namespace minty::rendering
{
	class PushConstantInfo
		: public Object
	{
	private:
		std::string _name;
		VkShaderStageFlags _stageFlags;
		uint32_t _offset;
		uint32_t _size;

	public:
		PushConstantInfo();

		PushConstantInfo(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const offset, uint32_t const size);

#pragma region Get

		std::string const& get_name() const;

		VkShaderStageFlags get_stage_flags() const;

		uint32_t get_offset() const;

		uint32_t get_size() const;

		VkPushConstantRange to_range() const;

#pragma endregion

	};
}