#pragma once

#include "M_Object.h"

#include <vector>

namespace minty::rendering
{
	class UniformConstantInfo
		: public Object
	{
	private:
		std::string _name;
		VkDescriptorType _type;
		uint32_t _set;
		uint32_t _binding;
		uint32_t _count;
		VkDeviceSize _size;
		VkShaderStageFlags _stageFlags;
		std::vector<ID> _ids;

	public:
		UniformConstantInfo();

		UniformConstantInfo(std::string const& name, VkShaderStageFlags const stageFlags, VkDescriptorType const type, VkDeviceSize const size, uint32_t const set, uint32_t const binding, uint32_t const count = 1, std::vector<ID> const& ids = {});

#pragma region Get

		std::string const& get_name() const;

		VkDescriptorType get_type() const;

		uint32_t get_set() const;

		uint32_t get_binding() const;

		uint32_t get_count() const;

		VkDeviceSize get_size() const;

		VkShaderStageFlags get_stage_flags() const;

		std::vector<ID> const& get_ids() const;

#pragma endregion
	};
}