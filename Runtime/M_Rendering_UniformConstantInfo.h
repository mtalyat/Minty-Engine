#pragma once

#include "M_Base.h"

#include <vector>

namespace minty::rendering
{
	/// <summary>
	/// Holds info for a uniform constant.
	/// </summary>
	struct UniformConstantInfo
	{
		/// <summary>
		/// The name of the uniform constant.
		/// </summary>
		std::string name;

		/// <summary>
		/// The type of uniform constant.
		/// </summary>
		VkDescriptorType type;

		/// <summary>
		/// The set this uniform constant belongs to.
		/// </summary>
		uint32_t set;

		/// <summary>
		/// The binding that this uniform constant belongs to.
		/// </summary>
		uint32_t binding;

		/// <summary>
		/// The number of elements in this uniform constant. Always 1 unless the uniform constant is an array.
		/// </summary>
		uint32_t count;

		/// <summary>
		/// The size of the uniform constant in bytes.
		/// </summary>
		VkDeviceSize size;

		/// <summary>
		/// The shader stage this uniform constant is part of.
		/// </summary>
		VkShaderStageFlags stageFlags;

		/// <summary>
		/// Any IDs needed, depending on the type.
		/// </summary>
		std::vector<ID> ids;
	};
}